//
// Created by bs on 13.02.18.
//

#ifndef MULTITHREAD_1_UNIVERSAL_CALL_H
#define MULTITHREAD_1_UNIVERSAL_CALL_H

#include<memory>
#include<algorithm>
#include<future>

template<class Callable>
struct universal_call;


template<class R, class...Args>
struct universal_call<R(Args...)> {
    template<typename Callable>
    universal_call(Callable&& callable)
            : _impl{std::make_shared<model<Callable>>(std::forward<Callable>(callable))}
    {

    }

    R operator()(Args&&...args) const
    {
        return _impl->call(std::forward<Args>(args)...);
    }
private:
    struct concept
    {
        virtual R call(Args&&...args) = 0;
        virtual ~concept() = default;
    };

    template<class Callable>
    struct model : concept
    {
        model(Callable&& callable)
                : _callable(std::move(callable))
        {

        }
        R call(Args&&...args) override
        {
            return _callable(std::forward<Args>(args)...);
        }
        Callable _callable;
    };

    std::shared_ptr<concept>_impl;
};


template<class R, class...Args>
struct universal_call<std::packaged_task<R(Args...)>>
: universal_call<void(Args...)>
{
using universal_call<void(Args...)>::universal_call;
};

// (possibly) helpful function
template<class F>
universal_call<F> make_universal_call(F&& f)
{
    return universal_call<F>(std::forward<F>(f));
}
#endif //MULTITHREAD_1_UNIVERSAL_CALL_H
