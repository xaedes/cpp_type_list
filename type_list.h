#pragma once

#include <tuple>
#include <type_traits>

// https://en.cppreference.com/w/cpp/types/disjunction
template<class...> struct disjunction : std::false_type { };
template<class B1> struct disjunction<B1> : B1 { };
template<class B1, class... Bn>
struct disjunction<B1, Bn...> 
    : std::conditional_t<bool(B1::value), B1, disjunction<Bn...>>  { };


// MIT License
// 
// Copyright (c) 2021 xaedes
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


template <class... Args>
struct type_list
{
    using size = std::integral_constant<std::size_t, sizeof...(Args)>;
  
    // https://en.cppreference.com/w/cpp/utility/tuple/tuple_element
    template <std::size_t N>
    using get = typename std::tuple_element<N, std::tuple<Args...>>::type;

    template <template <class> class F>
    using map = type_list<F<Args>...>;

    template <template <class...> class F>
    using apply = F<Args...>;

    template <class T>
    using has = disjunction<std::is_same<T, Args>...>;

    using tuple = apply<std::tuple>;

    template <
        class Needle, 
        size_t Idx=0
    >
    struct find : std::conditional_t<
        std::is_same<
            get<Idx>,
            Needle
        >::value,
        std::integral_constant<std::size_t, Idx>,
        find<Needle, Idx+1>
    > {};

    template <class... Args2>
    using zip = type_list<type_list<Args, Args2>...>;     
};
template <class type_list>
using tuple_of = typename type_list::tuple;
