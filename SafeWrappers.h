//
// Created by bs on 04.04.18.
//

#ifndef MULTITHREAD_1_SAFEWRAPPERS_H
#define MULTITHREAD_1_SAFEWRAPPERS_H

#include <iostream>
#include <sstream>
#include <istream>
#include <mutex>
#include <future>
#include <condition_variable>
#include <thread>




struct safeCout:public std::stringstream
{
    //inline
    std::mutex pmut;

    ~safeCout()
    {
        std::lock_guard<std::mutex>coutGuard(pmut);
        std::cout.rdbuf();
        std::cout.flush();
    }
};

struct safeCin:public std::istream
{
    //inline
    std::mutex cmut;

    ~safeCin()
    {
        std::lock_guard<std::mutex>cinGuard(cmut);
        std::cin.rdbuf();
        std::cin.clear();
    }
};



#endif //MULTITHREAD_1_SAFEWRAPPERS_H
