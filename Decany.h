//
// Created by bs on 31.03.18.
//

#ifndef MULTITHREAD_1_DECANY_H
#define MULTITHREAD_1_DECANY_H

#include<iostream>
#include<experimental/any>
#include<functional>

template<class T>
struct tag_t
{
    constexpr tag_t(){};
};

template<class T>
constexpr tag_t<T> tag{};

template<class any_method>
using any_sig_from_method = typename any_method::signature;
template<class any_method, class Sig=any_sig_from_method<any_method>>
struct any_method_function;
template<class any_method, class R, class...Args>
struct any_method_function<any_method, R(Args...)>
{
    template<class T>
    using decorate = std::conditional_t< any_method::is_const, T const, T >;
    using any = decorate<std::experimental::any>;
    using type = R(*)(any&, any_method const*, Args&&...);
    template<class T>
    type operator()( tag_t<T> )const{
        return +[](any& self, any_method const* method, Args&&...args) {
            return (*method)( std::experimental::any_cast<decorate<T>&>(self), decltype(args)(args)... );
        };
    }
};

template<class...any_methods>
using any_method_tuple = std::tuple< typename any_method_function<any_methods>::type... >;
template<class...any_methods, class T>
any_method_tuple<any_methods...> make_vtable( tag_t<T> ) {
    return std::make_tuple(
            any_method_function<any_methods>{}(tag<T>)...
    );
}
template<class...methods>
struct any_methods
{
    private:

    any_method_tuple<methods...> const* vtable = 0;

    template<class T>
    static any_method_tuple<methods...> const* get_vtable( tag_t<T> )
    {
        static const auto table = make_vtable<methods...>(tag<T>);
        return &table;
    }
public:
    any_methods() = default;

    template<class T>
    any_methods( tag_t<T> ): vtable(get_vtable(tag<T>)) {}

    any_methods& operator=(any_methods const&)=default;

    template<class T>
    void change_type( tag_t<T> ={} ) { vtable = get_vtable(tag<T>); }

    template<class any_method>
    auto get_invoker( tag_t<any_method> ={} ) const
    {
        return std::get<typename any_method_function<any_method>::type>( *vtable );
    }
};

template<class...methods>
struct DecanyT;

//This searches the methods that the super any supports for SFINAE and better error messages:

template<class Decany, class method>
struct super_method_applies_helper : std::false_type {};
template<class M0, class...Methods, class method>
struct super_method_applies_helper<DecanyT<M0, Methods...>, method> :
        std::integral_constant<bool, std::is_same<M0, method>{} ||
                                     super_method_applies_helper<DecanyT<Methods...>, method>{}>
{};
template<class...methods, class method>
auto super_method_test( DecanyT<methods...> const&, tag_t<method> )
{
    return std::integral_constant<bool, super_method_applies_helper< DecanyT<methods...>, method
    >{} && method::is_const >{};
}
template<class...methods, class method>
auto super_method_test( DecanyT<methods...>&, tag_t<method> )
{
    return std::integral_constant<bool, super_method_applies_helper< DecanyT<methods...>, method
    >{} >{};
}
template<class Decany, class method>
struct super_method_applies:
        decltype( super_method_test( std::declval<Decany>(), tag<method> ) )
{};
//Next we create the any_method type. An any_method is a pseudo-method-pointer. We create it globally and const ly
//using syntax like:
//const auto print=make_any_method( [](auto&&self, auto&&os){ os << self; } );
//or in C++17:

const any_method print=[](auto&&self, auto&&os){ os << self; };
//Note that using a non-lambda can make things hairy, as we use the type for a lookup step. This can be fixed, but
//would make this example longer than it already is. So always initialize an any method from a lambda, or from a
//type parametarized on a lambda.

template<class Sig, bool const_method, class F>
struct any_method {
    using signature=Sig;
    enum{is_const=const_method};
private:
    F f;
public:
    template<class Any,
// SFINAE testing that one of the Anys's matches this type:
            std::enable_if_t< super_method_applies< Any&&, any_method >{}, int>* =nullptr
    >
    friend auto operator->*( Any&& self, any_method const& m ) {
// we don't use the value of the any_method, because each any_method has
// a unique type (!) and we check that one of the auto*'s in the Decany
// already has a pointer to us. We then dispatch to the corresponding
// any_method_data...
        return [&self, invoke = self.get_invoker(tag<any_method>), m](auto&&...args)->decltype(auto)
        {
            return invoke( decltype(self)(self), &m, decltype(args)(args)... );
        };
    }
    any_method( F fin ):f(std::move(fin)) {}
    template<class...Args>
    decltype(auto) operator()(Args&&...args)const {
        return f(std::forward<Args>(args)...);
    }
};
//A factory method, not needed in C++17 I believe:
template<class Sig, bool is_const=false, class F>
any_method<Sig, is_const, std::decay_t<F>>
make_any_method( F&& f ) {
    return {std::forward<F>(f)};
}
//This is the augmented any . It is both an any , and it carries around a bundle of type-erasure function pointers that
//change whenever the contained any does:
template<class... methods>
struct DecanyT:std::experimental::any, any_methods<methods...> {
    using vtable=any_methods<methods...>;
public:
    template<class T,
            std::enable_if_t< !std::is_base_of<DecanyT, std::decay_t<T>>{}, int> =0
    >
    DecanyT( T&& t ):
            std::experimental::any( std::forward<T>(t) )
    {
        using dT=std::decay_t<T>;

        this->change_type( tag<dT> );
    }
    std::experimental::any& as_any()&{return *this;}
    std::experimental::any&& as_any()&&{return std::move(*this);}
    std::experimental::any const& as_any()const&{return *this;}
    DecanyT()=default;
    DecanyT(DecanyT&& o)noexcept:
            std::experimental::any( std::move( o.as_any() ) ),
            vtable(o)
    {}
    DecanyT(DecanyT const& o):
            std::experimental::any( o.as_any() ),
            vtable(o)
    {}
    template<class S,
            std::enable_if_t< std::is_same<std::decay_t<S>, DecanyT>{}, int> =0
    >
    DecanyT( S&& o )noexcept:
            std::experimental::any( std::forward<S>(o).as_any() ),
            vtable(o)
    {}
    DecanyT& operator=(DecanyT&&)=default;
    DecanyT& operator=(DecanyT const&)=default;

    template<class T,
            std::enable_if_t< !std::is_same<std::decay_t<T>, DecanyT>{}, int>* =nullptr>
    DecanyT& operator=( T&& t )
    {
        ((std::experimental::any&)*this) = std::forward<T>(t);
        using dT=std::decay_t<T>;
        this->change_type( tag<dT> );
        return *this;
    }
};
//Because we store the any_method s as const objects, this makes making a Decany a bit easier:

template<class...Ts>
using Decany = DecanyT< std::remove_cv_t<Ts>... >;

//Test code:
const auto oprint = make_any_method<void(std::ostream&)>([](auto&& p, std::ostream& t){ t << p <<
                                                                                         "\n"; });
const auto wprint = make_any_method<void(std::wostream&)>([](auto&& p, std::wostream& os ){ os << p
                                                                                               << L"\n"; });
int main()
{
    Decany<decltype(oprint), decltype(wprint)> a = 7;
    Decany<decltype(oprint), decltype(wprint)> a2 = 7;
    (a->*oprint)(std::cout);
    (a->*wprint)(std::wcout);
}


#endif //MULTITHREAD_1_DECANY_H
