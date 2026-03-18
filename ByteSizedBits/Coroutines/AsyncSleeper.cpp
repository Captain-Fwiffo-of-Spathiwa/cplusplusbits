#include "AsyncSleeper.h"
using namespace std;



simple_task AsyncSleeperCoroutine()
{
    cout << "\t\tInside coroutine: Blocking stuff... this text, a sleep, etc.\n";

    this_thread::sleep_for(chrono::seconds(1));

    cout << "\t\tInside coroutine: About to call co_await, which in this case\n";
    cout << "\t\twill lead to await_suspend(), so this coroutine will yield\n";
    cout << "\t\tback to whoever called task.resume() as soon as whatever's in\n";
    cout << "\t\tawait_suspend() has finished. Our await_suspend() will make a\n";
    cout << "\t\tdifferent thread and return quickly. That new thread will go\n";
    cout << "\t\toff and do its own thing.\n";

    co_await sleep_awaitable{ 3 };

    cout << "\t\tInside coroutine: Resumed after async sleep. I was called by\n";
    cout << "\t\tthe 2nd thread! A coroutine has no way of just interrupting\n";
    cout << "\t\tthe main thread to resume, and this example only works because\n";
    cout << "\t\tthere happened to be a spare thread to resume it!\n";
}

void MakeAndRunAsyncSleeperCoroutine()
{
    cout << "Setup started.\n";
    cout << "\t--> Calling coroutine.\n";

    simple_task task = AsyncSleeperCoroutine();

    cout << "Coroutine called, which returned a simple_task.\n";
    cout << "\t--> Calling task.resume()\n";

    task.resume();

    cout << "task.resume() returned. This means the coroutine did something but then also yielded or finished.\n";
    cout << "\t--> Checking if the coroutine is officially \"done\".\n";

    while (!task.done())
    {
        this_thread::sleep_for(chrono::seconds(1));
        cout << "\t--> (1 second later) Still not done.\n";
    }

    cout << "Coroutine done, since task.done() returned true.\n";
}





//#include <iostream>
//int main() {
//    std::cout << "_MSC_VER: " << _MSC_VER << std::endl;
//    std::cout << "_MSC_FULL_VER: " << _MSC_FULL_VER << std::endl;
//    return 0;
//}

//  https://simplifycpp.org/books/minibooklet/mini_booklet_Coroutines_The_Comprehensive_Concise_Guide.pdf

//#include <iostream>
//#include <coroutine>
//#include <thread>
//#include <chrono>

/* ------------------------------------------------------------------------
*   This example shows how a coroutine can pause itself while some
*   asynchronous work runs, let outside work also continue, then pick up
*   from where it paused when the internal async work was done.
*
*   1. main creates a new coroutine (MyCoroutine). You know it's a
*      coroutine because it has co_await in it.
*
*   2. Specifically because it is a coroutine, things work differently to
*      what you're used to when main calls MyCoroutine. Since it is a
*      coroutine, it doesn't actually execute anything immediately. It
*      first does some checks and setup and then maybe runs, or maybe
*      return before executing even a single line of code.
*
*      In this case, it returns before executing a single line of code.
*
*      That's because the compiler sees it is a coroutine (from co_await),
*      so it looks at its return type. In that return type, it looks for
*      a struct of type "promise_type". In that struct's definition, it
*      checks the function initial_suspend() to see what it returns, which
*      in this case is std::suspend_always. So the compiler understands
*      that the coroutine must always immediately suspend when it starts.
*
*   3. main() calls task.resume(), so MyCoroutine() resumes from its
*      last suspension point. Ie., the very start.
*
*   4. MyCoroutine() runs like normal, including a blocking sleep for 1
*      second, until it reaches co_await.
*
*   5. The line:
*
*          co_await sleep_awaitable{3};
*
*      ... says "Construct a temporary sleep_awaitable, then co_await it."
*      In that line, co_await is a keyword being used like:
*
*          return 7;
*
*      ... which says "Construct a temporary int, then return it."
*      Now, the line:
*
*          co_await expr;
*
*      ... basically means "run await_suspend() or run await_resume()".
*      It basically decides like this:
*
*           expr.await_ready()? thing.await_resume(): thing.await_suspend();
*
*      Basically.
*
*      Except the 3 functions await_ready()/suspend()/resume() can be
*      defined in several different places.
*
*      ==== MORE ON THAT ==================================================
*
*          When you use:
*               co_await expr;
*          ...then SOMETHING has to have defined implementations of:
*               await_ready(),
*               await_suspend(), and
*               await_resume().
*
*          The thing that has those 3 functions defined is our "awaiter".
*          That's what the co_await keyword runs on; an "awaiter", a thing
*          that has those 3 functions implemented.
*
*          But why do I say "something" defines them?
*
*          Because there are 3 different places they could be defined:
*               1. in the expr object itself
*               2. in the result of await_transform() in the promise
*               3. in the result of using an overloaded co_await _operator_
*
*          Any of those 3 could be what produces the awaiter.
*
*          Note 1: The "promise" referred to there is the promise_type
*                  struct in your coroutine function's return type.
*
*          Note 2: I emphasised the _operator_ up there because the
*                  co_await _keyword_ and the co_await _operator_ are two
*                  different things with the same name. ...  Yes.
*                  In other words, using the co_await operator is one way
*                  to generate an awaiter for the co_await keyword.
*
*          From wreien on Discord:
*               Basically yes. Think of it that operator co_await is the
*               bit to provide the user-customizable part of the keyword
*               co_await operation, but the operation itself does more
*               things.
*               I guess I should say also it's used for co_yield since
*               that uses (keyword operation) co_await under the hood.
*               
*          Note 3: This process of mucking around with expr until we get
*                  an awaiter is called normalising.
* 
*          Anyway, so the line:
*
*               co_await expr;
*
*          ... means "After normalising, use await_ready() to decide
*          whether to call await_suspend() or await_resume().
* 
*      ==== WHAT NORMALISING LOOKS LIKE ===================================
* 
*          Given the line:
*               co_await expr;
*  
*          1. Does the promise_type have the function await_transform?
*             If yes:
*                 v = promise.await_transform(expr)
*             Else:
*                 v = expr
*          2. If member operator co_await() is defined for v objects:
*                 Use the result of v.co_await() as the awaiter
*             Else if free operator co_await() is defined for v objects:
*                 Use the result of co_await(v) as the awaiter
*             Else:
*                 Use v as the awaiter
*          3. Now you have your expr normalised into an awaiter. Now
*             the co_await keyword runs on that, calling await_ready()
*             and such.
* 
*          A simpler way to put it is:
*             a) promise.await_transform is considered first (coroutine-specific hook)
*             b) then operator co_await on the resulting object (type-specific hook)
*             c) if no operator co_await, that object itself is the awaiter
* 
*          Hot tip: An awaitable is something that will be turned into an
*                   awaiter but isn't necessarily already an awaiter.
*                   The co_await keyword needs to be followed by an
*                   awaitable; something that will resolve to an awaiter.
*                   Assuming things compile, all awaiters are awaitables.
*
*                   Yes, this means you can't always tell if an object is
*                   an awaitable just by looking at it, because it depends
*                   on await_transform() or the co_await operator.
* 
*          Conceptually:
*
*          An awaitable is a request.
*          An awaiter is an execution contract.
*
*          Awaitables expresses intent: “I will want to await this thing.”
*
*      ====================================================================
*
*   6. In this case, there is no await_transform in the promise_type, and
*      there is no overload of the co_await operator anywhere, so our
*      expr - sleep_awaitable{3} - gets used directly as the awaiter. It
*      does resolve to having those 3 functions, so it's a valid awaiter.
*
*   7. From that, we follow: await_ready()? await_resume: await_suspend().
*      In our sleep_awaitable type, we can see that await_ready() always
*      returns false, so our coroutine always runs await_suspend() at this
*      point of calling co_await sleep_awaitable{x}.
*
*      Fun fact: That doesn't mean it actually stops. It literally does
*      anything that await_suspend() tells it to do! THEN yields.
*
*   8. The end is in sight.
*
*      In our case, await_suspend() fires up a new thread, detaches it,
*      and off it goes. And it's THAT thread that controls when the
*      coroutine resumes! This is like a hardware interrupt or something
*      that says "Ok I've finished my async work, you can resume whatever
*      async function had paused because it was awaiting me".
*
*      It's the fact that our coroutine function was causing something to
*      spin up on another thread that allowed it to yield control back to
*      the main thread. The line:
*
*           co_await sleep_awaitable{3};
*
*      ...was never actually saying "Ok now I, the main thread, can
*      continue working in the external calling function while this
*      function suspends here. It is literally just saying "This function
*      suspends here until something makes it resume, and I will block
*      until await_suspend is finished, and would you look at that,
*      await_suspend in this case happens to be non-blocking."
*
*
*      C++ Coroutines Summary: 
*      "Do this thing that I defined to do absolutely anything, or instead
*       do this other thing that I defined to do absolutely anything. And
*       decide between them based on this boolean function that I defined
*       to decide absolutely anything."
*
*   9. Anyway, so the line co_await sleep_awaitable{3}; runs.
* 
*      Since await_ready() was false, it ran await_suspend(). And since
*      await_suspend() returns void (and not false), the coroutine does
*      actually do something expected and genuinely suspends.
*
*      At the line co_await sleep_awaitable{3};, it blocks until
*      await_suspend() completes, and then it pauses (ie., suspends) the
*      coroutine there and yields. At last we have coroutine behaviour.
*
*      Our coroutine is suspended. Sure, another thread happens to be
*      doing some work that the coroutine started, but that's really a
*      separate thing. Our coroutine is officially suspended.
*
*      The calling thread - the one that called task.resume() - gets back
*      control and continues. The next line it runs, back in main(), is:
*
*           std::cout << "Main: Coroutine running\n";
*
*      Then it enters the while loop.
*      The upshot of everything up to now is that so far we have seen:
*
*      A. The program started
*      B. It printed "Coroutine: Blocking stuff" immediately
*      C. Nothing happened for 1 second
*      D. Suddenly it printed "Coroutine: Starting async sleep", then
*         "Main: Coroutine running", and then "Main: Doing other work"
*          all together in that order.
*
*  10. The only thing coroutiney that's left is task.done().
*      This is some actual normal C++ for a moment, and we simply look at
*      the done() function of the simple_task return type, which itself
*      calls coro.done(), where coro is a handle_type, which is a
*      coroutine_handle<promise_type>.
*
*      That much we can handle.
*
*      So when does coroutine_handle::done() return true?
*
*      When:
*       a) the coroutine's work is done, and
*       b) final_suspend has returned suspend_always
*
*      The coroutine's work is done when it either falls of the end of its
*      body, or hits co_return. When this happens, final_suspend() is
*      called.
*
*      If final_suspend() returns suspend_always, the coroutine sees
*      itself as both done, and suspended.
*
*      Otherwise, final_suspend returns suspend_never. The sneaky part is
*      that this actually means the coroutine is destroyed completely.
*      Returning suspend_never calls coroutine.destroy(), so its resources
*      are going to be reclaimed.
*
*      This means that you can't get a done() == true on a coroutine that
*      returned suspend_never, because the handle is now dead and there's
*      nothing to call done() on.
*
*      So remember that oddness: If final_suspend returns suspend_never,
*      the coroutine cleans itself up but then can't announce itself as
*      done, even though conceptually it is well and truly done.
*
*      And the coroutine that CAN announce itself as done is the one that
*      is still alive, and simply officially finally suspended, waiting to
*      be destroyed.
*
*      You use suspend_always basically so the coroutine can finish but
*      you want it kept alive so you can inspect its results. At some
*      point you'd then have to call destroy on it. This also means that
*      you control the timing of when it gets cleaned up.
*
*      You use suspend_never when you know you don't need it and are happy
*      to let it clean up itself in its own timing.
*
*      Remember that your coroutine could get left abandoned, suspended
*      at an early suspension point. This will not return done() == true,
*      and it also can't destroy itself because it can't reach
*      final_suspend(). So if you have an abandoned coroutine, you will
*      have to manually call destroy() on it somewhere, even if
*      final_suspend returns suspend_never.
*
*      tldr; Your coroutine's body finished and final_suspend() returns
*      suspend_always. That's it.
*
*      And that's the whole code!
*
* ------------------------------------------------------------------------*/


/* ------------------------------------------------------------------------
*   Aggregate Initialisation
*
*   High-level aggregate requirements (modern C++)
*
*   Something is an "aggregate" if:
*    - It is an array, or a class / struct with a simple layout style.
*    - No user - declared or inherited constructors.
*    - No virtual functions.
*    - No private/protected direct data members
*      (and no private/protected/virtual base-class complications).
*
*    So the key idea is: aggregates are “data-shaped” types initialized
*    directly, not “behavior-shaped” types initialized through ctors.
*
*   When you see braces in an object's definition, it's a braced init list:
*
*      BlahType blah { 3, 4};
*
*   If BlahType is an aggregate, then this is "aggregate initialisation".
*   If BlahType is not an aggregate, this it isn't. It's just a braced
*   init list, which will end up just finding the best matching
*   constructor. This will, however, still prevent narrowing (because you
*   used the braces) which is a good thing.
*
*   So you can typically prefer to use braced init lists all the time.
* ------------------------------------------------------------------------*/

//struct sleep_awaitable
//{
//    int seconds;
//
//    bool await_ready() const noexcept
//    { 
//        return false;
//    }
//
//    void await_suspend(std::coroutine_handle<> coroHandle)
//    {
//        int suspendSeconds = seconds;
//
//        std::thread([coroHandle, suspendSeconds]()
//            {
//                std::this_thread::sleep_for(std::chrono::seconds(suspendSeconds));
//                coroHandle.resume();
//            }).detach();
//    }
//
//    void await_resume() const noexcept
//    {}
//};
//
//struct simple_task
//{
//    struct promise_type;
//
//    using handle_type = std::coroutine_handle<promise_type>;
//    handle_type coro;
//
//    simple_task(handle_type handleType)
//        : coro(handleType)
//    {
//    }
//
//    ~simple_task()
//    {
//        if (coro)
//        {
//            coro.destroy();
//        }
//    }
//
//    void resume()
//    {
//        if (coro && !coro.done())
//        {
//            coro.resume();
//        }
//    }
//
//    bool done() const
//    {
//        return !coro || coro.done();
//    }
//
//    struct promise_type
//    {
//        simple_task get_return_object()
//        {
//            return simple_task{ handle_type::from_promise(*this) };
//        }
//
//        std::suspend_always initial_suspend()
//        {
//            return {};
//        }
//
//        std::suspend_always final_suspend() noexcept
//        {
//            return {};
//        }
//
//        void return_void()
//        {
//        }
//
//        void unhandled_exception()
//        {
//        }
//    };
//};
//
//simple_task MyCoroutine()
//{
//    std::cout << "Coroutine: Blocking stuff\n";
//    std::this_thread::sleep_for(std::chrono::seconds(1));
//    std::cout << "Coroutine: Starting async sleep\n";
//    co_await sleep_awaitable{ 3 };
//    std::cout << "Coroutine: Resumed after async sleep\n";
//}

//int main()
//{
//    simple_task task = MyCoroutine();
//    task.resume();
//    std::cout << "Main: Coroutine running\n";
//
//    while (!task.done())
//    {
//        std::cout << "Main: Doing other work\n";
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//    }
//
//    std::cout << "Main: Coroutine finished\n";
//}
