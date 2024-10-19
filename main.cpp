#include <iostream>

#include "factorial.cpp"
#include "allocator.h"
#include "myvector.h"

using namespace std;

int main() 
{
    map<int, int> factorials;
    for (int i = 0; i < 10; ++i)
        factorials[i] = factorial(i);

    map<int, int, less<int>, PoolAllocator<pair<const int, int>, 10>> factorialsCustomAlloc { factorials.begin(), factorials.end() };
    for (auto [num, numFact] : factorialsCustomAlloc)
        cout << num << " " << numFact << endl;

    MyVector<int> myvec;
    for (int i = 0; i < 10; ++i)
        myvec.push_back(factorial(i));

    MyVector<int, PoolAllocator<int, 10>> myvecCustomAlloc;
    copy(myvec.begin(), myvec.end(), back_inserter(myvecCustomAlloc));
    for (int val : myvecCustomAlloc)
        cout << val << endl;

    return 0;
}