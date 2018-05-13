//
// Created by bs on 13.02.18.
//

#ifndef MULTITHREAD_1_REGULARTYPE_H
#define MULTITH READ_1_REGULARTYPE_H

#include<iostream>
#include<memory>
#include<typeinfo>

using dtor_unique_ptr = std::unique_ptr<void, void(*)(void*)>;
template<class T, class...Args>
dtor_unique_ptr make_dtor_unique_ptr( Args&&... args ) {
    return {new T(std::forward<Args>(args)...), [](void* self){ delete static_cast<T*>(self); }};
}
struct regular_vtable {
    void(*copy_assign)(void* dest, void const* src); // T&=(T const&)
    void(*move_assign)(void* dest, void* src); // T&=(T&&)
    bool(*equals)(void const* lhs, void const* rhs); // T const&==T const&
    bool(*order)(void const* lhs, void const* rhs); // std::less<T>{}(T const&, T const&)
    std::size_t(*hash)(void const* self); // std::hash<T>{}(T const&)
    std::type_info const&(*type)(); // typeid(T)
    dtor_unique_ptr(*clone)(void const* self); // T(T const&)
};
template<class T>
regular_vtable make_regular_vtable() noexcept {
    return {
            [](void* dest, void const* src){ *static_cast<T*>(dest) = *static_cast<T const*>(src); },
            [](void* dest, void* src){ *static_cast<T*>(dest) = std::move(*static_cast<T*>(src)); },
            [](void const* lhs, void const* rhs){ return *static_cast<T const*>(lhs) == *static_cast<T
            const*>(rhs); },
            [](void const* lhs, void const* rhs) { return std::less<T>{}(*static_cast<T
            const*>(lhs),*static_cast<T const*>(rhs)); },
            [](void const* self){ return std::hash<T>{}(*static_cast<T const*>(self)); },
            []()->decltype(auto){ return typeid(T); },
            [](void const* self){ return make_dtor_unique_ptr<T>(*static_cast<T const*>(self)); }
    };
}

template<class T>
regular_vtable const* get_regular_vtable() noexcept {
    static const regular_vtable vtable=make_regular_vtable<T>();
    return &vtable;
}
struct regular_type {
    using self=regular_type;
    regular_vtable const* vtable = 0;
    dtor_unique_ptr ptr{nullptr, [](void*){}};
    bool empty() const { return !vtable; }
    template<class T, class...Args>
    void emplace( Args&&... args ) {
        ptr = make_dtor_unique_ptr<T>(std::forward<Args>(args)...);
        if (ptr)
            vtable = get_regular_vtable<T>();
        else
            vtable = nullptr;
    }
    friend bool operator==(regular_type const& lhs, regular_type const& rhs) {
        if (lhs.vtable != rhs.vtable) return false;
        return lhs.vtable->equals( lhs.ptr.get(), rhs.ptr.get() );
    }
    bool before(regular_type const& rhs) const {
        auto const& lhs = *this;
        if (!lhs.vtable || !rhs.vtable)
            return std::less<regular_vtable const*>{}(lhs.vtable,rhs.vtable);
        if (lhs.vtable != rhs.vtable)
            return lhs.vtable->type().before(rhs.vtable->type());
        return lhs.vtable->order( lhs.ptr.get(), rhs.ptr.get() );
    }
// technically friend bool operator< that calls before is also required
    std::type_info const* type() const {
        if (!vtable) return nullptr;
        return &vtable->type();
    }
    regular_type(regular_type&& o):
            vtable(o.vtable),
            ptr(std::move(o.ptr))
    {
        o.vtable = nullptr;
    }
    friend void swap(regular_type& lhs, regular_type& rhs){
        std::swap(lhs.ptr, rhs.ptr);
        std::swap(lhs.vtable, rhs.vtable);
    }
    regular_type& operator=(regular_type&& o) {
        if (o.vtable == vtable) {
            (ptr.get(), o.ptr.get());
            return *this;
        }
        auto tmp = std::move(o);
        swap(*this, tmp);
        return *this;
    }
    regular_type(regular_type const& o):
            vtable(o.vtable),
            ptr(o.vtable?o.vtable->clone(o.ptr.get()):dtor_unique_ptr{nullptr, [](void*){}})
    {
        if (!ptr && vtable) vtable = nullptr;
    }
    regular_type& operator=(regular_type const& o) {
        if (o.vtable == vtable) {
            vtable->copy_assign(ptr.get(), o.ptr.get());
            return *this;
        }
        auto tmp = o;
        swap(*this, tmp);
        return *this;
    }
    std::size_t hash() const {
        if (!vtable) return 0;
        return vtable->hash(ptr.get());
    }
    template<class T,
            std::enable_if_t< !std::is_same<std::decay_t<T>, regular_type>{}, int>* =nullptr
    >
    regular_type(T&& t) {
        emplace<std::decay_t<T>>(std::forward<T>(t));
    }
};
namespace std {
    template<>
    struct hash<regular_type> {
        std::size_t operator()( regular_type const& r )const {
            return r.hash();
        }
    };
    template<>
    struct less<regular_type> {
        bool operator()( regular_type const& lhs, regular_type const& rhs ) const {
            return lhs.before(rhs);
        }
    };
}
#endif //MULTITHREAD_1_REGULARTYPE_H
