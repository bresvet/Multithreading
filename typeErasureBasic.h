//
// Created by bs on 31.03.18.
//basic technique of type erasure using dynamic polymorphism

#ifndef MULTITHREAD_1_TYPEERASUREBASIC_H
#define MULTITHREAD_1_TYPEERASUREBASIC_H

#include <iostream>
#include <ostream>

class Printable
{
public:
    template <typename T>
    Printable(T value) : pValue(new Value<T>(value)) {}
    ~Printable() { delete pValue; }
    void print(std::ostream &os) const { pValue->print(os); }
    Printable(Printable const &)=delete;
    void operator = (Printable const &)=delete;

private:

    struct ValueBase
    {
        virtual ~ValueBase() = default;
        virtual void print(std::ostream &os) const=0;
    };

    template <typename T>
    struct Value : ValueBase
    {
        Value(T const &t) : v(t) {}
        virtual void print(std::ostream &os) const { os << v;}
        T v;
    };

    ValueBase *pValue;
};
/* in C++1x: =delete */; // not implemented
/* in C++1x: =delete */; // not implemented


#endif //MULTITHREAD_1_TYPEERASUREBASIC_H
