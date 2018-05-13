//
// Created by bs on 31.03.18.
//

#ifndef MULTITHREAD_1_ARRAY_VIEW_H
#define MULTITHREAD_1_ARRAY_VIEW_H


#include<iostream>
#include<type_traits>

template<class T>
using data_t = decltype( std::declval<T>().data() );

template<class Src, class T>
using compatible_data = std::integral_constant<bool, std::is_same< data_t<Src>, T* >{}
|| std::is_same< data_t<Src>, std::remove_const_t<T>* >{}>;

template<class T>
struct array_view
{
// the core of the class:
    T* b=nullptr;
    T* e=nullptr;
    T* begin() const { return b; }
    T* end() const { return e; }
// provide the expected methods of a good contiguous range:
    T* data() const { return begin(); }
    bool empty() const { return begin()==end(); }
    std::size_t size() const { return end()-begin(); }
    T& operator[](std::size_t i)const{ return begin()[i]; }
    T& front()const{ return *begin(); }
    T& back()const{ return *(end()-1); }
// useful helpers that let you generate other ranges from this one
// quickly and safely:
    array_view without_front( std::size_t i=1 ) const {
        i = (std::min)(i, size());
        return {begin()+i, end()};
    }
    array_view without_back( std::size_t i=1 ) const {
        i = (std::min)(i, size());
        return {begin(), end()-i};
    }
    // array_view is plain old data, so default copy:
    array_view(array_view const&)=default;
// generates a null, empty range:
    array_view()=default;
// final constructor:
    array_view(T* s, T* f):b(s),e(f) {}
// start and length is useful in my experience:
    array_view(T* s, std::size_t length):array_view(s, s+length) {}
// SFINAE constructor that takes any .data() supporting container
// or other range in one fell swoop:

    template<class Src,
            std::enable_if_t< compatible_data<std::remove_reference_t<Src>&, T >{}, int>* =nullptr,
    std::enable_if_t< !std::is_same<std::decay_t<Src>, array_view >{}, int>* =nullptr
    >
    array_view( Src&& src ):
            array_view( src.data(), src.size() )
    {}
// array constructor:

    template<std::size_t N>
    array_view( T(&arr)[N]):array_view(arr,N) {}

// initializer list, allowing {} based:

    template<class U,
            std::enable_if_t< std::is_same<const U, T>{}, int>* =nullptr>
    array_view( std::initializer_list<U> il ):array_view(il.begin(), il.end()) {}
};

#endif //MULTITHREAD_1_ARRAY_VIEW_H
