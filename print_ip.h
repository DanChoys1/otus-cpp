#ifndef PRINT_IP_H
#define PRINT_IP_H

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <tuple>

using namespace std;

namespace
{
constexpr char Delimetr = '.';
}

/// integral type
template<typename IntegrT, class = enable_if_t<is_integral_v<IntegrT>>>
void print_ip(const IntegrT val)
{
    bool isFirsteIter = true;
    for (int i = sizeof(IntegrT) - 1; i >= 0; --i)
    {
        if (isFirsteIter)
            isFirsteIter = false;
        else
            cout << Delimetr;

        cout << ( (val >> 8*i) & 0xFF );
    }

    cout << endl;
}


template<class, class = void>
struct HasCStrFunc : std::false_type {};
template<class T>
struct HasCStrFunc<T, std::void_t<decltype(printf("%s", std::declval<T>().c_str()))>> : std::true_type {};

/// string type
template<typename StrT, class = enable_if_t<HasCStrFunc<StrT>::value>>
void print_ip(const StrT& str, int)
{
    printf("%s\n", str.c_str());
}

template<class, class = void>
struct HasPrintableBegin : std::false_type {};
template<class T>
struct HasPrintableBegin<T, std::void_t<decltype(cout << (*std::declval<T>().begin()))>> : std::true_type {};

/// container type
template<typename ContainerT, class = enable_if_t<HasPrintableBegin<ContainerT>::value>>
void print_ip(const ContainerT& con, long)
{
    bool isFirsteIter = true;
    for(const auto& v : con)
    {
        if (isFirsteIter)
            isFirsteIter = false;
        else
            cout << Delimetr;

        cout << v;
    }

    cout << endl;
}

// solve string and container conflict
template<typename ConOrStrT, class = enable_if_t<HasPrintableBegin<ConOrStrT>::value>>
void print_ip(const ConOrStrT& conOrStr)
{
    print_ip(conOrStr, int{});
}

/// tuple type
template<typename Type>
void print_ip(const tuple<Type>& t)
{        
    cout << get<0>(t) << endl;
}

/// tuple type
template<size_t index = 0, typename Type1, typename ...Types, class = enable_if_t<conjunction_v<is_same<Type1, Types>...>>>
void print_ip(const tuple<Type1, Types...>& t)
{        
    if constexpr (index < (sizeof...(Types)+1)) 
    {
        if (index != 0)
            cout << Delimetr;
        cout << get<index>(t);

        print_ip<index+1>(t);
    }

    if (index == 0)
        cout << endl;
}

#endif
