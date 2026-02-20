#include <iostream>

void DoTheThing(void (*someFunction)())
{
    someFunction();
}

int main()
{
    /*
    * Example of a lambda that holds state across scope. 
    * It makes another lambda inside itself and returns that lambda, and that
    * inner lambda has a persistent variable. Lambdas must use their capture
    * clause to set up variables to use for persistent state.
    * 
    * Be careful: If the inner lambda had captured by reference instead, then
    * the validity of the incrementing integer it returns would depend on the
    * source variable, not the lambda. In this case, the source `count` would
    * have already gone out of scope, so capturing by reference would be
    * broken, yet still compile and give undefined behaviour as it holds onto
    * a reference to place in memory that is no longer assigned to a variable.
    */ 
    auto make_incrementer = []()
    {
        int count = 0;
        return [count]() mutable { return ++count; };
    };

    /*
    * The lambda outputs another lambda, which we're storing here.
    * The lambda it returns is a lambda that had made its own copy of count.
    * It keeps and knows about that copy forever now; as long as that lambda
    * itself is still alive. And its 'count' variable is indeed just a copy,
    * so there's no care for whether the original source 'count' went out of
    * scope or not.
    *
    * The outer lambda - the one we assigned to make_incrementer - wouldn't
    * have kept a persistent state; it has no persistent variables.
    */
    auto increment = make_incrementer();

    int a = increment(); // a is 1
    int b = increment(); // b is 2
    // 'count' is now encapsulated inside the lambda; a and b do not see it

    std::cout << "Variable a is: " << a << " and variable b is: " << b << ".\n";

    // Example of a lamba with no () or return type
    DoTheThing([] {std::cout << "I did the thing.\n"; });

}
