//
// Created by bs on 01.03.18.
//

#ifndef MULTITHREAD_1_BUBBLESORT_H
#define MULTITHREAD_1_BUBBLESORT_H

#include<chrono>
#include<iostream>

template<typename Iterator>
void bubbleSort(Iterator start,Iterator finish)
{
    unsigned des=0;
    // std::lock_guard<std::mutex>bubbleSortGuard(vecMutex);
    for(int ei=0;ei<std::distance(start,finish);ei++)
    {
        for(Iterator ii=start;std::prev(ii,des)<finish;ii++)
        {
            if((*ii)>(*std::next(ii)))
            {
                auto temp =*ii;
                *ii = *(ii + 1);
                *(ii + 1) = temp;
            }
        }
        des++;
    }
}

#endif //MULTITHREAD_1_BUBBLESORT_H
