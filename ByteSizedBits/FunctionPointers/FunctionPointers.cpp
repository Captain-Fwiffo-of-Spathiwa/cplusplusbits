#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

// FUTURE SELF NOTE - We're continue with the ways to declare functions that
// return function pointers. I'm up to approach 5 (below), looking at decltype
// from the StackOverflow link given.


/*
    Today's Topic: Some Function Pointer Challenges
    
    Ref: https://stackoverflow.com/questions/997821/how-to-make-a-function-return-a-pointer-to-a-function-c

    Q1: Write a function that returns a function pointer.

    Store function pointers in a struct and call them.
    Use function pointers for callbacks in a simple event system.
    Replace a switch statement with a table of function pointers.
    Implement a menu system where each option is a function pointer.



*/


enum class Animal
{
    Cat,
    Dog
};

string GetNumCats()
{
    return "Too many cats.";
}

string GetNumDogs()
{
    return "Not enough doggos!";
}

string GetNumPuppies(bool doWolvesCount)
{
    return doWolvesCount ? "Ok maybe enough pups!" : "Never enough puppers!";
}


/* ------------------------------------------------------------------------
 *  This function is never used. It's just an example of calling the
 *  desired GetNum___() function without using function pointers.
 * -----------------------------------------------------------------------*/
string GetHowManyPets(Animal species)
{
    if (species == Animal::Cat)
    {
        return GetNumCats();
    }

    return GetNumDogs();
}

/*
    APPROACH 1: THE OLD UGLY WAY
 
    string (*GetPetCountingFunction(Animal species))() { // ... }

    This syntax is unpleasant. It's nearly 30 years old. It says:

        "GetPetCountingFunction is a real function that takes an Animal
         as its 1 paramater and returns a function pointer. The returned
         pointer is of the type: 'a function that takes no parameters
         and returns a string'."
   
    Breaking that down:

    GetPetCountingFunction  <-- The name of the real function we're defining.

    *GetPetCountingFunction <-- By putting a * to the left of its name, we're
                                now saying "This function's return type is
                                actually going to be a function pointer".

                                This is a critical difference to how we
                                normally look at function signatures. When we
                                see that *, we have to think "Ok, the return
                                type is a function pointer, and NOT whatever
                                is on the very left of the signature.
                                
    (Animal species)        <-- Parameter list of the real function. In this
                                case, it's GetPetCountingFunction's parameter
                                list.

    So if you look at JUST this part of the signature:

        (*GetPetCountingFunction(Animal species))

    ... it's not too hard to say "Ok, so GetPetCountingFunction is a function
    that takes an Animal and returns a function pointer." We can at least say
    that little understanding is contained inside the ( ).

    Then we just look outside the ( ) to see what type of function pointer it
    is. Ie., what is type of the functions that the pointer can point to?

    In this case, we look outside those ( ) and see:

    string ... ()           <-- So the returned function pointer is a pointer
                                to functions of type that take no parameters
                                and return a string.

                                In case it wasn't already clear, just like how
                                you can't copy an int* to a char*, you can't
                                copy a pointer for one type of function to a
                                pointer for a different type of function.
                                
                                So for example, GetPetCountingFunction returns
                                a pointer to functions of type string ().
                                Meanwhile, GetNumPuppies is a function of type
                                string (bool). 
                                
                                Therefore, you could never store a pointer to
                                GetNumPuppies in the same variable that you
                                store what GetPetCountingFunction returns.

*/
string (*GetPtrToPetCountingFunction(Animal species))()
{
    if (species == Animal::Cat)
    {
        return &GetNumCats;
    }

    return &GetNumDogs;
}

/*
    APPROACH 2: THE OLD WAY BUT A LITTLE LESS UGLY WITH C, ver 1.

    So we were going to write this, but it's ugly:

        string (*GetPetCountingFunction(Animal species))() { // ... }

    Instead, we can use typedef to clean it a little:

        typedef string (MyFunctionType) ();

    This says:

        "MyFunctionType is a type that is 'a function that returns a string
         and takes zero parameters'."

    And now we can make the GetPetCountingFunction signature a little
    more readable:

        MyFunctionType* GetPetCountingFunction(Animal species) { // ... }

    This is much easier to recognise. "Oh yeah, it takes an Animal and returns
    a pointer to a MyFunctionType".
*/

/*
    APPROACH 3: THE OLD WAY BUT A LITTLE LESS UGLY WITH C, ver 2.

    So could also include the pointer in the typedef, just for that extra step:

        typedef string (*PtrToMyFunctionType) ();

    Which means our signature becomes:

        PtrToMyFunctionType GetPetCountingFunction(Animal species) { // ... }

    Which is better? I prefer the former (ver. 1), since the pointer is more
    communicated, but the real answer is "Whatever the surrounding code uses".
    In reality, the time has long passed to be writing typedef in new C++ code.
*/

/*
    APPROACH 4: USING USING (C++ ONLY)

    This does exactly the same as what typedef does except:
     - a) it is more intuitive to write
     - b) using is more powerful than typedef, you can replace it completely
     - c) using is C++ only

     So instead of:

        typedef string (MyFunctionType) ();

    ... we instead use:

        using MyFunctionType = string();

    Now we can just read it intuitively. "MyFunctionType is a function that
    returns a string and takes no parameters".

    You can use this approach to include the pointer itself as well, just like
    you could with the typedef:

        using PtrToMyFunctionType = string(*)();

    "PtrToMyFunctionType is a pointer to a function that returns a string and
    takes no parameters".
    That's definitely not as intuitive to read from left to right. I'd just
    leave the pointer out of the using.
*/

/*
    APPROACH 5: DECLTYPE WITH C++11

    Dan, continue from https://stackoverflow.com/questions/997821/how-to-make-a-function-return-a-pointer-to-a-function-c


*/
int main()
{
    // To use our function-pointer-returning function, we can assign
    // its result to a function pointer and use that to execute:
    string(*myFuncPtr)() = GetPtrToPetCountingFunction(Animal::Cat);
    cout << myFuncPtr() << endl;

    // Or we can just execute that returned result directly:
    cout << GetPtrToPetCountingFunction(Animal::Dog)() << endl;
}
