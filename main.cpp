
    //
// Created by bs on 04.02.18.
//

#include<iostream>
#include<thread>
#include<vector>
#include<list>
#include<algorithm>
#include<cassert>
#include<functional>
#include<utility>
#include<type_traits>
#include<deque>
#include<stack>
#include<chrono>
#include<random>
#include<future>
#include<cmath>
#include<shared_mutex>
#include"mergeSort.h"
#include"bubbleSort.h"
#include"heapSort.h"
#include"radixSort.h"
#include"Semaphore.h"
#include"insertionSort.h"
#include"shellSort.h"
#include"task.h"
#include"ThreadPool.h"
#include"ThreadJoiner.h"
#include "SafeWrappers.h"
#include "quickSort.h"
#include <sstream>

  /*  template<typename T>
    void multi_push_back(const std::vector<T> &v,T&&...t)
    {
        (v.push_back(t),...);
    }
*/
    void simple_tcout(std::thread::id id)
    {
        std::cout<<"hello from: "<<id<<std::endl;
    }

    void safe_tcout(std::thread::id id)
    {
        safeCout{}<<"hello from "<<id<<std::endl;
    }

    template<typename T>
    void simple_tcin(T &&data,std::istream &o)
    {
        o>>data;
    }

    template<typename T>
    void safe_tcin(T &&data,std::istream &o)
    {
        safeCin{}>>data;
    }

    template<typename Iterator>
    void stlHeapSort(Iterator start,Iterator finish);

    template<typename Iterator>
    void stlStandardSort(Iterator start,Iterator finish);


    std::shared_timed_mutex stm;

    std::vector<int>v(1000);
    using std::cout;
    using std::endl;

    int comain()
    {


        std::default_random_engine generator;
        std::uniform_int_distribution<unsigned>distributor(0,1000);

       //thread_local
        for(auto &vp:v)
            vp=distributor(generator);

      /*  for(auto &vp:v)
          cout<<vp<<" ";
        cout<<endl;*/

        using Iterator=std::vector<int>::iterator;


        task<void(Iterator,Iterator)>stlHeapSortTask(std::move(stlHeapSort<Iterator>));
        task<void(Iterator,Iterator)>stlStandardSortTask(std::move(stlStandardSort<Iterator>));
        task<void(Iterator,Iterator)>bubbleSortTask(std::move(bubbleSort<Iterator>));
        task<void(Iterator,Iterator)>insertionSortTask(std::move(insertionSort<Iterator>));
        task<void(Iterator,Iterator)>quickSortTask(std::move(quickSort<Iterator>));

        //selection sort,heapSort,radixSort,shellSort,(mergeSort,quicksort)

        std::deque<task<void(Iterator,Iterator)>>sortingTasks;
                sortingTasks.push_back(std::move(stlHeapSortTask));
                sortingTasks.push_back(std::move(stlStandardSortTask));
                sortingTasks.push_back(std::move(bubbleSortTask));
                sortingTasks.push_back(std::move(insertionSortTask));
                sortingTasks.push_back(std::move(quickSortTask));

       std::unique_ptr<ThreadPool<void,Iterator,Iterator>>sortingPool
               =std::make_unique<ThreadPool<void,Iterator,Iterator>>(std::move(sortingTasks));

       std::vector<std::thread>cthreads;
        std::vector<std::thread>sthreads;


        for(size_t s=0;s<10;s++)
       {
           cthreads.emplace_back(std::thread(simple_tcout,std::this_thread::get_id()));
       }
       for(auto &cpv:cthreads)
       {
           cpv.join();
       }

       for(size_t s=0;s<10;s++)
       {
           sthreads.emplace_back(std::thread(safe_tcout,std::this_thread::get_id()));
       }


        for(auto &spv:sthreads)
        {
           spv.join();
        }



        std::unique_ptr<ThreadPool<void,Iterator,Iterator>>fepool
                =std::make_unique<ThreadPool<void,Iterator,Iterator>>();

     //   fepool->enqueue(std::move(stlHeapSortTask));
       // fepool->enqueue(std::move(stlStandardSortTask),std::chrono::milliseconds(1000));
        //fepool->enqueue(std::move(bubbleSortTask),std::chrono::milliseconds(1000),[&]()->bool{fepool->isEmpty();});

      sortingPool->start<5>(v.begin(),v.end());
      //sortingPool->start<4>(v.begin(),v.end());
      //sortingPool->start<4>(v.begin(),v.end());
      //sortingPool->start<4>(v.begin(),v.end());





          /*  auto promise=std::promise<ThreadPool<void,Iterator,Iterator>>();
            auto worker=std::thread([&](){promise.set_value(std::move(*sortingPool));});

            auto consumer=[&](){promise.get_future();};

           consumer();

*/
       // auto promise=std::promise<ThreadPool<void,Iterator,Iterator>>();
       // auto worker=std::thread{[&]{promise.set_value(std::move(sortingPool));}};
       // auto recipient=std::thread{[&]{promise.get_future();}};

       // ThreadJoiner wjoiner{std::move(worker)};
       // ThreadJoiner rjoiner{std::move(recipient)};

      /*  std::thread stlHeapSortVectorThread{stlHeapSort};
        std::thread stlStandardSortVectorThread{stlStandardSort};
        std::thread bubbleVectorThread{bubbleSort};
        std::thread insertionVectorThread{insertionSort};


        //  auto asort=std::async(std::launch::async,std::sort<std::vector<int>::iterator>,v.begin(),v.end());

        std::unique_lock<std::mutex>ulbubble(vecMutex,std::defer_lock);
      std::unique_lock<std::mutex>ulstl(vecMutex,std::defer_lock);
      std::unique_lock<std::mutex>ulinsertion(vecMutex,std::defer_lock);
      std::unique_lock<std::mutex>ulprint(vecMutex,std::defer_lock);
        */
      //  ulstl.lock();
      //  stlsortVectorThread.join();
       // threadJoiner stlJoiner(std::move(stlsortVectorThread));
      //  ulstl.unlock();




      //  ulbubble.lock();
       // bubbleVectorThread.join();
       // threadJoiner bubbleJoiner(std::move(bubbleVectorThread));
       // ulbubble.unlock();


      //  ulinsertion.lock();
        //threadJoiner insertionJoiner(std::move(insertionVectorThread));
        //insertionVectorThread.join();
       // ulinsertion.unlock();



      //  ulprint.lock();
       // threadJoiner printJoiner(std::move(printVectorThread));
       // ulprint.unlock();

        //   bubbleVectorThread.detach();
       //asort.get();


       // mergeSort(v);
       // mergeSort(l);

      // cout<<endl<<std::boolalpha<<isContainer<std::vector<int>>::value<<endl
        //   << isContainer<std::list<int>>::value<<endl<<isContainer<std::deque<double>>::value<<endl
          //                                                            <<isContainer<std::stack<void*>>::value<<endl;

      /*  std::thread bubbleVectorThread{bubble,v.begin(),v.end()};
        //std::thread mergeVectorThread{};
        std::thread stlsortVectorThread{std::sort,v.begin(),v.end()};

       std::thread bubbleListThread{bubble,l.begin(),l.end()};
        //std::thread mergeListThread{};
        std::thread stlsortListThread{std::sort,l.begin(),l.end()};

*/
        return 0;
    }


    int main()
    {
        comain();

    }



    template<typename Iterator>
    void stlStandardSort(Iterator start,Iterator finish)
    {
        auto stl_start=std::chrono::system_clock::now();
        std::sort(start,finish);
        auto stl_end=std::chrono::system_clock::now();

        std::chrono::duration<double>stl_duration=stl_end-stl_start;
        std::cout<<std::endl<<std::chrono::duration_cast<std::chrono::milliseconds>(stl_duration).count()<<std::endl;

    }

    template<typename Iterator>
    void stlHeapSort(Iterator start,Iterator finish)
    {
        auto stl_start=std::chrono::system_clock::now();
        std::sort_heap(start,finish);
        auto stl_end=std::chrono::system_clock::now();

        std::chrono::duration<double>stl_duration=stl_end-stl_start;
        std::cout<<std::endl<<std::chrono::duration_cast<std::chrono::milliseconds>(stl_duration).count()<<std::endl;
    }








