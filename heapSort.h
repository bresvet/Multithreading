//
// Created by bs on 01.03.18.
//

#ifndef MULTITHREAD_1_HEAPSORT_H
#define MULTITHREAD_1_HEAPSORT_H

#include<iostream>
#include<algorithm>
#include<type_traits>
#include<memory>

template<bool B>
struct BoolConstant
{
    static auto value=BoolConstant<B>::val;
    static constexpr auto val=value;
};

using TrueType=BoolConstant<true>;
using FalseType=BoolConstant<false>;

template<typename...>
struct Conjunction : std::true_type {};

template<typename B1>
struct Conjunction<B1> : B1 { };

template<typename B1, typename... Bn>
struct Conjunction<B1, Bn...>
        : std::conditional_t<bool(B1::value),Conjunction<Bn...>,B1>{};

template<typename...>
struct Alternative:std::true_type{};

template<typename B1>
struct Alternative<B1>:public B1{};

template<typename B1,typename...Bn>
struct Alternative<B1,Bn...>:public std::conditional_t<B1::value,std::true_type,Alternative<Bn...>>{};

template<typename B1>
struct Negation:public std::conditional_t<B1::value,std::false_type,std::true_type>{};


template<typename...>
using VoidT=void;

template<typename,typename=VoidT<>>
struct isIterator:public std::false_type{};

template<typename I>
struct isIterator<I,VoidT<Conjunction<typename I::value_type,typename I::difference_type
        ,typename I::pointer,typename I::reference>>>:public std::true_type{};

template<typename Iterator>
size_t indexFromIterator(Iterator it,Iterator begin)
{
    return std::distance(it,begin);
}

template<typename T,typename Iterator=typename std::vector<T>::iterator>
class Heap:private std::vector<T>
{
public:

    explicit Heap()= default;
    explicit Heap(unsigned n)
            :std::vector<T>(n)
    {

    }

    void bubbleUp(Iterator elem)
    {
            while(indexFromIterator(elem,this->top())<this->getSize() && *elem>*(parent(elem)))
            {
                this->swap(elem,parent(elem));
            }
    }

    void bubbleDown(Iterator elem)
    {
     while(true)
     {
         if(*(leftChild(elem))>*elem and onWhichLevel(elem)!=getLevelsQuantity())
         {
             this->swap(elem,leftChild(elem));
             continue;
         }
         if(*(rightChild(elem))>*elem and onWhichLevel(elem)!=getLevelsQuantity())
         {
             this->swap(elem,rightChild(elem));
             continue;
         }
         if((*(leftChild(elem))<=*elem && *(rightChild(elem))<=*elem ) or onWhichLevel(elem)==getLevelsQuantity())
             break;
     }
    }

    unsigned long getSize()const
    {
        return std::vector<T>::size();
    }

    unsigned getLevelsQuantity()const
    {
        return std::log2(this->getSize())+1;
    }


    Iterator top()const
    {
       return std::vector<T>::begin();
    }

    Iterator last()const
    {
        return std::vector<T>::end();
    }

    Iterator parent(Iterator elem)const
    {
        return std::prev(elem,indexFromIterator(elem,this->top())/2);
    }

    Iterator leftChild(Iterator elem)const
    {
        return std::next(elem,onWhichLevel(elem));
    }

    Iterator rightChild(Iterator elem)const
    {
        return std::next(elem,onWhichLevel(elem)+1);
    }

    void swap(Iterator elem1,Iterator elem2)
    {
        std::iter_swap(elem1,elem2);
    }

    unsigned onWhichLevel(Iterator elem)const
    {
        return std::log2(indexFromIterator(elem,this->top()));
    }

    void erase(Iterator elem)
    {
        std::vector<T>::erase(elem);
    }

    Iterator getAndRemoveLastNode()
    {
        auto end=this->end();
        std::vector<T>::pop_back();
        return end;
    }

    //explicit operator Heap<T>(std::vector<T>)

    Heap &operator=(const std::vector<T>&vec)
    {
        *this=vec;
        return *this;
    }

    Heap &operator=(std::vector<T>&&vec)
    {
        *this=std::move(vec);
        return *this;
    }

    void push_back(T &&elem)
    {
        std::vector<T>::push_back(elem);
    }

};

template<typename Iterator>
void heapSort(Iterator start,Iterator finish)
{
    using T=typename Iterator::value_type;

    std::vector<T> sorted;
    Heap<T> heap = Heap<T>();
    for (auto it = start; it<finish; it++)
        heap.push_back(*it);
    while (heap.getSize())
    {
        heap.bubbleDown(heap.top());
        heap.swap(heap.top(), heap.last());
        sorted.push_back(heap.getAndRemoveLastNode());
    }
    for(auto it=sorted.begin();it<sorted.end();it++)
        *(start+std::distance(start,it))=*it;
}

#endif //MULTITHREAD_1_HEAPSORT_H
