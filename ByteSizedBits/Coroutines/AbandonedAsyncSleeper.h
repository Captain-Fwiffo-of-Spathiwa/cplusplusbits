#pragma once

#include <iostream>
#include <coroutine>
#include <thread>
#include <chrono>

struct AbandonedSleeperTask
{
    struct promise_type;
    using handle_type = coroutine_handle<promise_type>;

    handle_type CoroHandle;

    AbandonedSleeperTask(handle_type coroHandle) : CoroHandle(coroHandle) {}
    ~AbandonedSleeperTask() { if (CoroHandle) { CoroHandle.destroy(); } }

    
    
    struct promise_type
    {
        /*-----------------------------------------------------------------
           The typical scenario is that despite writing these 5 functions
           that define a promise_type, you don't actually call them. You
           are simply providing them so the compiler can set them up for
           the coroutine machinery to call at runtime.
           You will rarely (or maybe never) need to refer to the promise
           instance directly in your code.
          ----------------------------------------------------------------*/

        /*-----------------------------------------------------------------
           get_return_object() is called only once, just after the
           coroutine is built. Its single job is to produce the thing your
           coroutine function returns when the developer calls it. You'll
           notice that coroutine functions never actually make the thing
           that they return. get_return_object() is what does.

           Fun fact: get_return_object() doesn't need to return exactly
           what your coroutine function returns. get_return_object() only
           needs to return something that can be used to automatically
           construct what your coroutine function returns. Even in the
           simple example here, get_return_object() is just returning a
           coroutine handle instead of an actual AbandondedSleeperTask,
           purely because C++ can make that conversion (from handle to
           AbandonedSleeperTask) automatically, thus allowing your
           coroutine function to still return an AbandonedSleeperTask.

           Is it good practice to do that? Probably not. It's a bit less
           code but really only makes things less clear for no advantage.
          ----------------------------------------------------------------*/
        handle_type get_return_object()
        {
            return handle_type::from_promise(*this);
        }

        // note, see which is called first
        /*-----------------------------------------------------------------
           initial_suspend() is what the coroutine machinery calls at the
           moment of starting to execute the coroutine function. The
           runtime uses the result of initial_suspend() to decide if the
           coroutine function's code will actually start to execute
           immediately (ie., when point you called the coroutine) or not.

           The return type of initial_suspend() is an awaiter, which means
           it's something that has await_ready(), await_resume() and
           await_suspend() implemented. I won't go through all those here;
           this is only to say that that's what initial_suspend() is
           doing: it's returning an awaiter that the coroutine machinery
           uses to decide if the coroutine should start executing now or
           not. What's the point of that, you might ask? Isn't it the job
           of initial_suspend() itself to make that decision? Why bother
           with yet another layer of yet more custom decision making?

           And the answer is typically yes. initial_suspend() is typically
           what should be making this decision rather than mucking around
           with making it return a custom awaiter that complicates things
           further. That's why it's normal for initial_suspend() to simply
           return std::suspend_always or std::suspend_never.

           But indeed, initial_suspend() can return a custom awaiter, and
           you can code your custom awaiter to do absolutely any nonsense.
           It can define its await_ready(), await_suspend(), and
           await_resume() to have no sensible resume/suspend logic at all.

           So like most other coroutine things, initial_suspend is just
           a what-gets-called and when-it-gets-called by the coroutine
           machinery. What it's conventionally supposed to do is one thing
           (which is to sensibly control the coroutine's flow). But what
           it actually does is completely up to the developer.
         -----------------------------------------------------------------*/
        std::suspend_always initial_suspend()
        { 
            return {}; 
        }

        /*-----------------------------------------------------------------
           final_suspend() is the same as initial_suspend() but at the
           other end. A way I like to see it is this:

           Task MyCoroutineFunction()
           {
           << initial_suspend() is fired at this point >>


               // stuff
               co_await/co_yield/co_return ...
               // stuff


           << final_suspend() is fired at this point >>
           }

           You use final_suspend to decide whether - at the point of
           hitting its closing brace - the coroutine should sit there
           suspend, or should go destroy itself. It's commom to keep the
           coroutine alive so that its result can keep being queried.
           
           Once again, this is its conventional role but what it actually
           does can be completely illogical and can be any other operation
           imaginable if you feel like doing so in your own awaiter.

           final_suspend() has to be noexcept, since at the point that
           final_suspend() is called in the coroutine function, C++'s
           exception handling opportunity has already passed, so the
           standard says that your final_suspend() should simply not be
           allowed to throw exceptions.
         -----------------------------------------------------------------*/
        std::suspend_always initial_suspend()
        { 
            return {}; 
        }
    };

};



AbandonedSleeperTask Create_AbandonedSleeperCoroutine();
void Start_AbandonedSleeperCoroutine();
void Resume_AbandonedSleeperCoroutine();