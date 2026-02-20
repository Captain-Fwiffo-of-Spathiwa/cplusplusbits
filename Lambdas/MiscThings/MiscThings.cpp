#include <iostream>

const int& TryToPassThrough(const int& x)
{
    std::cout << "X inside the function is: " << x << "\n";
    return x;
}



int main()
{
    std::cout << "Hello World!\n";


    const int& ref = TryToPassThrough(42);

    std::cout << "ref outside the function is: " << ref << "\n";

}

