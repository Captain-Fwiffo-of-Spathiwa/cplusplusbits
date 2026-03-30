#pragma once

#include <iostream>
#include <coroutine>
#include <thread>
#include <chrono>



struct sleep_awaitable
{
    int seconds;

    bool await_ready() const noexcept
    {
        return false;
    }

    void await_suspend(std::coroutine_handle<> coroHandle)
    {
        int suspendSeconds = seconds;

        std::thread([coroHandle, suspendSeconds]()
            {
                std::this_thread::sleep_for(std::chrono::seconds(suspendSeconds));
                coroHandle.resume();
            }).detach();
    }

    void await_resume() const noexcept
    {
    }
};

struct simple_task
{
    struct promise_type;

    using handle_type = std::coroutine_handle<promise_type>;
    handle_type coro;

    simple_task(handle_type handleType) : coro(handleType) {}
    ~simple_task() { if (coro) { coro.destroy(); } }
    void resume() { if (coro && !coro.done()) { coro.resume(); } }
    bool done() const { return !coro || coro.done(); }

    struct promise_type
    {
        simple_task get_return_object() { return simple_task{ handle_type::from_promise(*this) }; }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };
};

simple_task AsyncSleeperCoroutine();
void MakeAndRunAsyncSleeperCoroutine();