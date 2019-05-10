#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <string>
#include <uniset.hpp>

using namespace std;

template <typename T, std::size_t N>
void printSizeAndElems(const typename UniversalSet<T, N>::Vector &v)
{
    std::cout << "Size: " << v.count() << std::endl;
    for(auto &e : v) std::cout << e << ", ";
    std::cout << std::endl;
}

void uniTest();

#endif // TEST_H
