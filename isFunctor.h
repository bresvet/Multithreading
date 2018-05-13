//
// Created by bs on 13.02.18.
//

#ifndef MULTITHREAD_1_ISFUNCTOR_H
#define MULTITHREAD_1_ISFUNCTOR_H

#include<type_traits>

template<typename...>
using VoidT=void;

template<typename,typename=VoidT<>>
struct isFunctor:std::false_type
{

};

template<typename F>
struct isFunctor<F,VoidT<decltype(std::declval<F>()())>>:std::true_type
{

};


#endif //MULTITHREAD_1_ISFUNCTOR_H
