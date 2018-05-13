//
// Created by bs on 20.02.18.
//
#include<iostream>
#include<thread>
#include<condition_variable>
#include<mutex>

#ifndef MULTITHREAD_1_SEMAPHORE_H
#define MULTITHREAD_1_SEMAPHORE_H

template<typename T>
class Semaphore
{

};

template<>
class Semaphore<int>
{
public:

    explicit Semaphore(unsigned count=0)
    :count(count)
    {

    }

    inline void notify(unsigned tid)
    {
        std::unique_lock<std::mutex>nl(m);
        count++;
        std::cout<<"notify from "<<tid<<std::endl;
        cv.notify_one();
    }

    inline void wait(unsigned tid)
    {
        std::unique_lock<std::mutex>wl(m);
        while(!count)
        {
            std::cout << "wait from " << tid << std::endl;
            cv.wait(wl);
            std::cout<<"running: "<<tid<<std::endl;
        }
            count++;
    }



private:
    unsigned count;
    std::mutex m;
    std::condition_variable cv;
};

template<>
class Semaphore<bool>
{
public:

    explicit Semaphore(bool condition)
            :condition(condition)
    {

    }

    inline void notify(unsigned tid)
    {
        std::unique_lock<std::mutex>nl(m);
        condition=true;
        std::cout<<"notify from "<<tid<<std::endl;
        cv.notify_one();
    }

    inline void wait(unsigned tid)
    {
        std::unique_lock<std::mutex>wl(m);
        while(!condition)
        {
            std::cout << "wait from " << tid << std::endl;
            cv.wait(wl);
            std::cout<<"running: "<<tid<<std::endl;
        }
        condition=true;
    }



private:
    bool condition;
    std::mutex m;
    std::condition_variable cv;
};


#endif //MULTITHREAD_1_SEMAPHORE_H
