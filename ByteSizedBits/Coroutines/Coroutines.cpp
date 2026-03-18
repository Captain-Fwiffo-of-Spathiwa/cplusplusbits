#include <iostream>
#include <coroutine>
#include <thread>
#include <chrono>
#include <syncstream>

// boilerplate promise type required for c plus plus 20 coroutines
struct FireAndForget {
    struct promise_type {
        FireAndForget get_return_object() { return {}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };
};

// awaitable helper to move execution to a background thread
struct MoveToBackground {
    bool await_ready() { return false; }

    void await_suspend(std::coroutine_handle<> h) {
        // launch a new thread and immediately resume the coroutine on it
        std::thread([h]() {
            h.resume();
            }).detach();
    }

    void await_resume() {}
};

FireAndForget AsyncTimer() {
    // switch context to a background thread
    co_await MoveToBackground{};

    // this sleep happens on the background thread
    std::this_thread::sleep_for(std::chrono::seconds(4));

    // synchronized output to prevent text collision
    std::osyncstream(std::cout) << "Time's up\n";
}

//int main() {
//    std::cout << "Starting\n";
//
//    // fire the coroutine
//    AsyncTimer();
//
//    // main thread work loop
//    for (int i = 1; i <= 7; ++i) {
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//        std::osyncstream(std::cout) << i << " seconds passed\n";
//    }
//
//    std::cout << "Finished\n";
//    return 0;
//}