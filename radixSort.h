//
// Created by bs on 01.03.18.
//
#include<iostream>
#include<algorithm>
#include<thread>
#include<vector>
#include<array>
#include<iterator>
#include<queue>

#ifndef MULTITHREAD_1_RADIXSORT_H


unsigned digitAt(long long unsigned num, unsigned pos)
{
    for(unsigned i=0;i<pos;i++)
        num/=10;

    return num%10;
}

unsigned digitsQuantity(long long unsigned num)
{
    unsigned counter=0;

    while(num)
    {
     num/=10;
     counter++;
    }

    return counter;
}

template<typename Iterator>
void radixSort(Iterator start,Iterator finish)
{
    long long elementsQuantity = std::distance(start, finish);
    using T=typename std::iterator_traits<Iterator>::value_type;

    std::array<std::queue<T>, 10> buckets;

    T max = *(std::max_element(start, finish));

    for (unsigned s = 0; s < digitsQuantity(max); s++)
    {
        for (auto it = start; it < finish; it++)
        {
            (buckets[digitAt(*it, s)]).push(*it);
        }
    }

    for(unsigned s=0;s<digitsQuantity(max);s++)
    {
        for(unsigned t=0;t<buckets[s].size();t++)
        {
        *(std::next(start,s))=(buckets[s]).at(t);
        }
    }
}


#define MULTITHREAD_1_RADIXSORT_H
#endif //MULTITHREAD_1_RADIXSORT_H
