//
// Created by bs on 01.03.18.
//

#ifndef MULTITHREAD_1_QUICKSORT_H
#define MULTITHREAD_1_QUICKSORT_H
#include <type_traits>
#include <iterator>
#include <future>


template<typename Iterator>
void quickSort(Iterator start,Iterator finish)
{
    Iterator mid=std::next(start,std::distance(start,finish));
    using VType=typename std::iterator_traits<Iterator>::value_type;

    auto pivot=[&]()
    {
        VType biggest=std::max(*start,std::max(*mid,*finish));
        if(biggest==*start)
        return *mid>*finish?mid:finish;
        if(biggest==*mid)
        return *start>*finish?start:finish;
        if(biggest==*finish)
        return *start>*mid?start:mid;
    };


    if(start < finish)
    {

        auto sortFirstFuture = std::async(std::launch::async, [&]() { quickSort(start, pivot());});
        auto sortSecondFurure = std::async(std::launch::async,[&]() { quickSort(pivot()+ 1,finish);});
        sortFirstFuture.get();
        sortSecondFurure.get();
    }
}


#endif //MULTITHREAD_1_QUICKSORT_H
