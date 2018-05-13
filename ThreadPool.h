//
// Created by bs on 13.02.18.
//

#ifndef MULTITHREAD_1_THREADPOOL_H
#define MULTITHREAD_1_THREADPOOL_H

#include<iostream>
#include<memory>
#include<mutex>
#include<algorithm>
#include"task.h"
#include<future>
#include<deque>
#include"isFunctor.h"

template<typename R,typename ...Args>
class ThreadPool
{
public:

    using DurationT=std::chrono::duration<double_t>;
    using MS=std::chrono::milliseconds;
    using ulock=std::unique_lock<std::mutex>;
    //using Iterator=std::vector<int>::iterator;

    template<typename Mutex>
    void lockFor(const Mutex &mut, MS time)
    {
        auto lockingStart = std::chrono::steady_clock::now();
        mut.lock();
        std::this_thread::sleep_until(lockingStart + time);
        mut.unlock();
    }


    explicit ThreadPool(std::deque<task<R(Args...)>> &&jobs)
            : jobs(std::move(jobs))
    {

    }

    explicit ThreadPool()=default;



    std::mutex mut;
    std::timed_mutex tmut;

    std::deque<task<R(Args...)>> jobs;
    std::vector<std::future<R>> results;
    std::condition_variable cv;


    std::future<R> enqueue(task<R(Args...)> &&f)
    {
        std::packaged_task<R(Args...)> p(std::forward<task<R(Args...)>>(f));
        auto fut = p.get_future();

        {
            std::lock_guard<std::mutex> sloguar(mut);
            jobs.emplace_back(std::move(p));
        }
        cv.notify_one();
        return fut;
    }

    std::future<R> enqueue(task<R(Args...)>&&f,DurationT timeLimit)
    {
        std::packaged_task<R(Args...)>p(std::forward<task<R(Args...)>>(f));
        auto fut=p.get_future();

        {
            std::unique_lock<std::mutex> ul(mut);
            jobs.emplace_back(std::move(p));
            cv.wait_for(ul, timeLimit);
        }
        if(fut.valid())
            return fut;
    }

    template<typename Predicate>
    std::future<R> enqueue(task<R(Args...)>&&f,DurationT timeLimit,Predicate pred)
    {

        std::packaged_task<R(Args...)>p(std::forward<task<R(Args...)>>(f));
        auto fut=p.get_future();
        {
            std::unique_lock<std::mutex> ul(mut);
            jobs.emplace_back(std::move(p));
            auto start = std::chrono::steady_clock::now();

            cv.wait(ul, [&]() -> bool { pred() || std::chrono::steady_clock::now()-start>timeLimit;});
        }

        if(fut.valid())
            return fut;
    }



template<size_t N=1>
void start(Args...args)
{
    for(size_t s=0;s<N;s++)
    {
        results.push_back(std::async(std::launch::async,[&]{thread_F(args...);}));
    }
}

void abort()
{
    cancel_pending();
    finish();
}

void cancel_pending()
{
    std::unique_lock<std::mutex>ul(mut);
    jobs.clear();
}

void finish()
{
    {
        std::unique_lock<std::mutex> ul(mut);
        for (auto &&unused:results)
        {
            jobs.push_back({}); }
    }
    cv.notify_all();
    results.clear();

}

ThreadPool(ThreadPool&&)noexcept= default;
ThreadPool(const ThreadPool&)=delete;
ThreadPool &operator=(const ThreadPool&)=delete;

~ThreadPool()
{
    finish();
}

bool isEmpty()const
{
    return jobs.empty();
}
private:
void thread_F(Args...args)
{
    while(true)
    {
        task<R(Args...)>f;
        {
            std::unique_lock<std::mutex>ul(mut);
            if(jobs.empty())
                cv.wait(ul,[&]()->bool{return !jobs.empty();});

        }
        f=std::move(jobs.front());
        jobs.pop_front();
        if(!f.isValid()) return;
        f(args...);
    }
}


};

#endif //MULTITHREAD_1_THREADPOOL_H

