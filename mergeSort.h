//
// Created by bs on 13.02.18.
//

#ifndef MULTITHREAD_1_SORTSALGOS_H
#define MULTITHREAD_1_SORTSALGOS_H

#include<iostream>
#include<type_traits>

template<typename Iterator>
void merge(Iterator start1,Iterator finish1,Iterator start2,Iterator finish2)
{
    using Type=typename std::iterator_traits<Iterator>::value_type;
    std::vector<Type>helpVec;

    while(helpVec.size()!=(std::distance(start1,finish1)+std::distance(start2,finish2)))
    {
        unsigned lshCounter=0;
        unsigned rshCoutner=0;

        Type lv=*(std::next(start1,lshCounter));
        Type rv=*(std::next(start2,rshCoutner));

        if(lv>rv)
        {
            helpVec.push_back(lv);
            lshCounter++;
        }
        else
        {
            helpVec.push_back(rv);
            rshCoutner++;
        }
    }

    for(Iterator it=start1;it<finish2;it++)
    {
        auto whichPos=std::distance(it,start1);
        *it=helpVec.at(whichPos);
    }
}

template<typename Iterator>
void mergeSort1(Iterator start,Iterator finish)
{
    Iterator mid=start+(finish-start)/2;

    auto future1
            = std::async(std::launch::deferred,[&]()
            {
                mergeSort1(start,mid);
            });

    auto future2
            = std::async(std::launch::deferred, [&]()
            {
                mergeSort1(mid+1,finish) ;
            });

    future1.get();
    future2.get();

    merge(start,mid,mid+1,finish);
}

template<typename...>
using VoidT=void;

template<typename,typename=VoidT<>>
struct isPair:public std::false_type{};

template<typename T>
struct isPair<T,VoidT<decltype(typename T::first())>>:public std::true_type{};


template<typename Iterator>
void swapWithIterators(Iterator first,Iterator second)
{
    auto temp=*first;
    *first=*second;
    *second=temp;
}

template<typename Iterator>
void swapInContainer(Iterator first,Iterator second)
{
    auto dis=std::distance(first,second);
    for(auto it=first;it<second;it++)
    {
        swapWithIterators(it,it+dis);
    }
}


//template<typename T>
//auto merge=[&](T t1,T t2)-> decltype(auto){return t1>t2?std::make_pair(t1,t2):std::make_pair(t2,t1);};



template<typename Iterator>
void mergeSort(Iterator start,Iterator finish)
{
    assert(!std::distance(start,finish));
    unsigned neededMerges = static_cast<unsigned>(std::distance(start, finish) / 2 + 1);
    unsigned counter=0;
    for(auto it=start;it<finish;it+=pow(2,counter))
    {
        swapInContainer(it,it+pow(2,counter));
        counter++;
    }
    while(counter<neededMerges)
    {
        mergeSort(start,finish);
    }

}
#endif //MULTITHREAD_1_SORTSALGOS_H
