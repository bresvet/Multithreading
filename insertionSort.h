//
// Created by bs on 01.03.18.
//

#ifndef MULTITHREAD_1_INSERTIONSORT_H
#define MULTITHREAD_1_INSERTIONSORT_H
#include<iostream>
#include<chrono>

template<typename Iterator>
void insertionSort(Iterator start,Iterator finish)
{
    auto insertion_start=std::chrono::system_clock::now();
    for(Iterator it=start+1;it<finish;it++)
    {
        auto helpit=it;
        while(*helpit<*(helpit-1))
        {
            int temp=*(helpit-1);
            *(helpit-1)=*helpit;
            *helpit=temp;

            helpit--;
        }
    }
    auto insertion_end=std::chrono::system_clock::now();

    std::chrono::duration<double>insertion_duration=insertion_end-insertion_start;
    std::cout<<std::endl<<std::chrono::duration_cast<std::chrono::milliseconds>(insertion_duration).count()<<std::endl;
}


#endif //MULTITHREAD_1_INSERTIONSORT_H
