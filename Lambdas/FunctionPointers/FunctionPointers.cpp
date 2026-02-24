#include <iostream>
using namespace std;



void SayHello()
{
    cout << "Oh hai" << endl;
}


int main()
{
    /*
        So you want a function pointer. That means you're saying:

            "I want a pointer variable that holds the memory address of
             a function. I'll be using that pointer to call the function."
             
        Function pointers have a fundamental difference to normal
        variable pointers: implicit pointer conversion. 

        You can't dereference a function pointer and just access its data
        as if it's a data variable. A function never was an object that
        holds data, so while you might have a function variable (ie., the
        function name by itself), or a dereferenced function pointer,
        you can't read it as if it's data. Consider, for example, that
        data variable have a size defined by the type. Functions don't
        have a size that makes sense.

        So what happens if you try to dereference one?

        You do get an lvalue - something that stuff can be assigned
        to - but that lvalue is a function DESIGNATOR, like a handle.

        In other words, a dereferenced function pointer still gives you just
        a reference to the function, like an access ticket, and as soon as
        you try to use it, C/C++ just uses the pointer anyway.

        This implicit conversion that C/C++ does is quite literal; you really
        can try to deference a function pointer and end up just getting the
        pointer, or getting the behaviour the pointer would have invoked.
        
        Like this:

        myFuncPtr(param1, param2);

        ...or like this:

        (*myFuncPtr)(param1, param2);

        ...or even like all these:

        (**myFuncPtr)(param1, param2);
        (***myFuncPtr)(param1, param2);
        (****myFuncPtr)(param1, param2);

        C/C++ just implicitly converts function designators back to
        function pointers in a call context.

        - - - - - - - - - - - -

        Even if you make a function pointer variable and want to assign
        another function pointer's value to it, it is still just as loose:

        int CountCats(); 
        
        myFuncPtr = CountCats;
        myFuncPtr = *CountCats;
        myFuncPtr = &CountCats;
        myFuncPtr = *****CountCats;

        They're all the same!

        How can you even tell if something is function or a function pointer?
        You can at least use the following:

        static_assert(std::is_same_v<decltype(*p), void(&)()>, "Type is void(&)()");

        This says:

            "Assert that *p is a type that returns a void, is a reference, and
             takes no parameters."

        That means it's a function designator. As close to saying "the actual
        function" as you are able to.

        And alternatively:

        static_assert(std::is_same_v<decltype(p), void(&)()>, "Type is void(*)()");

        This says:

            "Assert that p is a type that returns a void, is a pointer, and
             takes no parameters."

        So that one's definitely a function pointer.

        The moral of the story is that when you work with function pointers,
        just keep thinking in function pointers. Don't worry about
        dereferencing, or about when you're dereferencing. You don't have to
        think in "pointer brain". You just use function pointers the way
        everyone uses function pointers, ie.,:
        
        ...to store a address of some function:

        myFuncPtr = someOtherFunctionName;

        ...and to call the function at that address:

        int numBirds = myFuncPtr(param1, param2);

        So now we know how to call functions from function pointers.

        -------------------------------------------------------------------

        HOW TO DECLARE A FUNCTION POINTER
        ---------------------------------

        void (*someFunction)(int, char);
    
        Yes, this is the basic syntax for making a pointer that points to
        a function. Yes, it's unintuitive.

        BUT, think of it like this:

            "When I declare a data type pointer, I put the * to the left
             of the variable name."

            "And when I declare a function pointer, I put the * to the left
             of the function name."

        Eg:

        int* myIntPtr;                    // Declare a pointer to an int data type 
        int *myIntPtr;                    // Same thing
        void (*myFuncPtr)(int, char);     // Declare a pointer to a void(int,char) function type

        So my thinking is "The * goes to the right of the type."
        But many people declare pointers like "The * goes to the left of the variable name."
        And if you follow that second approach, that works for function pointers too.

        Note that if we'd taken out the *, we're actually just declaring a
        real function, because this:

        void (someFunction)();

        ...is exactly the same as this:

        void someFunction();

        So now we know how to declare function pointers.
    */

    void (*myFunctionPtr)();

    myFunctionPtr = SayHello;

    cout << "1\n";
    myFunctionPtr();
    
    cout << "2\n";
    (*myFunctionPtr)();
    
    cout << "3\n";
    (&SayHello)();
    
    cout << "4\n";
    (SayHello)();
    
    cout << "5\n";
    (*SayHello)();
    
    cout << "6\n";
    (**SayHello)();
    
    cout << "7\n";
    (***SayHello)();

    cout << "8\n";
    (************SayHello)();

    myFunctionPtr = SayHello;   // These are all exactly the same!
    myFunctionPtr = &SayHello;
    myFunctionPtr = *SayHello;
    myFunctionPtr = ****SayHello;


    SayHello();                 // 1   These are also all exactly the same!
    (SayHello)();               // 2
    (&SayHello)();              // 3
    (*SayHello)();              // 4
    (**SayHello)();             // 5
    (************SayHello)();   // 6

    myFunctionPtr();            // 7
    (myFunctionPtr)();          // 8
    (*myFunctionPtr)();         // 9
    (***myFunctionPtr)();       // 10
    (*******myFunctionPtr)();   // 11
    (&(*myFunctionPtr))();      // 12
}

/*

So what you could learn from here:

1.a) How to use `using` to simplify function pointer declarations (so they feel more
     like normal datatype pointer declarations.

1.b) How to use typedef as well if you want.

2. How to load a DLL's function into a function pointer, conditionally check
   it for null, and if it is, put a safe stub function in your function
   pointer instead.

3. Read MyFunction and read &MyFunction and see if you get the same value.

4. Using #define to #define the names of functions from DLLs you're loading
   as the names of function's you've written, so you can use those DLLs'
   function names in your code, but they're resolving to your functions,
   which were in turn set to point to the loaded function if valid or your
   safe stub function if not. 

   Like this:

    XInputGetState = (MyFunctionType*)GetProcAddress(xInputLibrary, "XInputGetState");
    if (!XInputGetState)
    {
        XInputGetState = SafelyDoNothingStub;
    }

*/

// DECLARING FUNCTION POINTERS  (Take 2)


void SomeRealFunction()
{
    // Do stuff
}

#include <functional>

void DemoCode()
{


    // Approach 1: Classic syntax
    void(*myFuncPtr)();
    myFuncPtr = &SomeRealFunction;
    myFuncPtr = SomeRealFunction;  // Same thing, more readable
    
    // Or
    void(*myFuncPtr2)() = &SomeRealFunction;
    void(*myFuncPtr3)() = SomeRealFunction;  // Same thing, more readable

    // Approach 3: auto
    auto myFuncPtr4 = &SomeRealFunction;
    auto myFuncPtr5 = SomeRealFunction;

    // Note - See if these are both function pointers in the end

    // Approach 4: Use std::function
    // Note: A heavier object, not just a function pointer or designator. A wrapper.
    // Note: It needs the <functional> include.
    // Note: Can do a lot more than just hold a function. Uses type erasure. Can
    //       hold a function, lambda, functor, or bind expression. More for later.
    std::function<void()> myFuncPtr6 = &SomeRealFunction;
    std::function<void()> myFuncPtr6 = SomeRealFunction;


}
