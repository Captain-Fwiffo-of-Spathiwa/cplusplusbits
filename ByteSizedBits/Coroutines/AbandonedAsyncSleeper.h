#pragma once

#include <iostream>
#include <coroutine>
#include <thread>
#include <chrono>

struct AbandonedSleeperTask
{
    struct promise_type;

    using handle_type = std::coroutine_handle<promise_type>;
    handle_type coro;

    AbandonedSleeperTask(handle_type handleType)
        : coro(handleType)
    {
    }

    ~AbandonedSleeperTask()
    {
        if (coro)
        {
            coro.destroy();
        }
    }

    void resume()
    {
        if (coro && !coro.done())
        {
            coro.resume();
        }
    }

    bool done() const
    {
        return !coro || coro.done();
    }

    struct promise_type
    {
        AbandonedSleeperTask get_return_object()
        {
            return AbandonedSleeperTask{ handle_type::from_promise(*this) };
        }

        std::suspend_always initial_suspend()
        {
            return {};
        }

        std::suspend_always final_suspend() noexcept
        {
            return {};
        }

        void return_void()
        {
        }

        void unhandled_exception()
        {
        }
    };
};



AbandonedSleeperTask Create_AbandonedSleeperCoroutine();
void Start_AbandonedSleeperCoroutine();
void Resume_AbandonedSleeperCoroutine();