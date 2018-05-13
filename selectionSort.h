//
// Created by bs on 11.03.18.
//
#include<iostream>
#include<vector>
#include<type_traits>
#include <algorithm>

#ifndef MULTITHREAD_1_SELECTIONSORT_H
#define MULTITHREAD_1_SELECTIONSORT_H


template<typename Iterator>
void selectionSort(Iterator start,Iterator finish)
{
    using CType=typename std::iterator_traits<Iterator>::value_type;

    std::vector<CType>unsorted;
    std::vector<CType>sorted;

    for(auto it=start;it<finish;it++)
        unsorted.push_back(*it);



    while(!unsorted.empty())
    {
        auto min=std::min_element(unsorted.begin(),unsorted.end());
        sorted.push_back(*min);
        unsorted.erase(min);
    }
    std::copy(sorted.begin(),sorted.end(),start);
}

#endif //MULTITHREAD_1_SELECTIONSORT_H
