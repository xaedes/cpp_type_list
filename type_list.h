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


// https://en.cppreference.com/w/cpp/utility/tuple/tuple_element
template <class... Args>
struct type_list
{
    using size = std::integral_constant<std::size_t, sizeof...(Args)>;

    template <std::size_t N>
    using get = typename std::tuple_element<N, std::tuple<Args...>>::type;

    template <template <class> class F>
    using map = type_list<F<Args>...>;

    template <template <class...> class F>
    using apply = F<Args...>;

    template <class T>
    using has = disjunction<std::is_same<T, Args>...>;

    using tuple = apply<std::tuple>;

    using begin = get<0>;
    using back  = get<size::value-1>;

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
    using zip_args = type_list<type_list<Args, Args2>...>;

    template <class other_type_list>
    using zip = typename other_type_list::template apply<zip_args>;

    template <class... Args2>
    using prepend_args = type_list<Args2..., Args...>;

    template <class other_type_list>
    using prepend = typename other_type_list::template apply<prepend_args>;

    template <class... Args2>
    using concat_args = type_list<Args..., Args2...>;

    template <class other_type_list>
    using concat = typename other_type_list::template apply<concat_args>;

    template <template <class, class> class F, class Lhs>
    using foldl = multi_object_tracker::foldl<F,Lhs,Args...>;
};
template <class type_list>
using tuple_of = typename type_list::tuple;



// https://gist.github.com/ntessore/dc17769676fb3c6daa1f
template<typename T, T... Ints>
struct integer_sequence
{
    typedef T value_type;
    static constexpr std::size_t size() { return sizeof...(Ints); }

    using as_type_list = type_list<std::integral_constant<T, Ints>...>;
};

template<std::size_t... Ints>
using index_sequence = integer_sequence<std::size_t, Ints...>;

template<typename T, std::size_t N, T... Is>
struct make_integer_sequence : make_integer_sequence<T, N-1, N-1, Is...> {};

template<typename T, T... Is>
struct make_integer_sequence<T, 0, Is...> : integer_sequence<T, Is...> {};

template<std::size_t N>
using make_index_sequence = make_integer_sequence<std::size_t, N>;

template<typename... T>
using index_sequence_for = make_index_sequence<sizeof...(T)>;

template<typename T>
using index_sequence_for_type_list = make_index_sequence<T::size::value>;
