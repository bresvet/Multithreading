//
// Created by bs on 13.02.18.
//

#ifndef MULTITHREAD_1_THREADJOINER_H
#define MULTITHREAD_1_THREADJOINER_H

#include<iostream>
#include<algorithm>
#include<thread>

class ThreadJoiner
{
public:
    ThreadJoiner(std::thread t)
            :t(std::move(t))
    {

    }
    ~ThreadJoiner()
    {
        if (t.joinable())
            t.join();
    }
private:
    std::thread t;
};

#endif //MULTITHREAD_1_THREADJOINER_H
