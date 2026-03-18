// I use this file for random little tests.

#include <iostream>
using std::cout;
using std::endl;



int GetOne(int& thing1)
{
    thing1 = 7;
    return 1;
}

int GetTwo(int& const thing2)  // Shouldn't compile but does with MSVC
{
    thing2 = 8;
    return 2;
}

void UseArray1(int arr[])
{
    cout << arr[0] << endl;
    cout << *arr << endl;
}

void UseArray2(int* arr)
{
    cout << arr[0] << endl;
    cout << *arr << endl;
}

void UseArray3(int *arr)
{
    cout << arr[0] << endl;
    cout << *arr << endl;
}



int main()
{
    int x = 1;
    int y = 2;

    cout << "GetOne returns: " << GetOne(x) << endl;
    cout << "GetTwo returns: " << GetTwo(y) << endl;
    cout << "x is now: " << x << endl;
    cout << "y is now: " << y << endl;

    UseArray1(&x);
    UseArray2(&x);
    UseArray3(&x);
}








/*
Just for fun, who can try to
a) explain the differences between these?
b) identify which are typically pointless to use?
c) identify which won't compile?
d) identify which are exactly the same?


// Round 1

int GetSomething();
const int GetSomething();
const int* GetSomething();
int* const GetSomething();


// Round 2

int GetSomething(int);
int GetSomething(const int);
int GetSomething(const int*);
int GetSomething(int const *);
int GetSomething(int* const);
int GetSomething(const int* const);
int GetSomething(int const * const);


// Round 3

int GetSomething(int&);
int GetSomething(int& const);
int GetSomething(const int&);
int GetSomething(const int& const);


// Round 4

int& GetSomething();
const int& GetSomething();


// Round 5

int GetSomething() const;
static int GetSomething() const;

*/