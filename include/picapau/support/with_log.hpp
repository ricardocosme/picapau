
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>
#include <type_traits>
#include <utility>

namespace picapau { 

struct with_log_base {};

template<typename T>
struct is_with_log : std::is_base_of<with_log_base, T> {};
    
template<typename T>
struct with_log : with_log_base
{
    using value_type = T;
    
    with_log() = default;
    with_log(T v, std::string l = "")
        : value(std::move(v))
        , log(std::move(l))
    {}
    
    static std::string logln(std::string label, std::string msg)
    { return "[" + label + "] " + msg + "\n"; }
    
    T value;
    std::string log;
};
        
template<typename WithLog,
         typename F,
         typename Ret = typename std::result_of<
             typename std::decay<F>::type(typename WithLog::value_type&)
         >::type
>
inline Ret mbind(WithLog& m1, F&& f,
                 typename std::enable_if<is_with_log<WithLog>::value>::type* = 0)
{
    auto m2 = f(m1.value);
    return {std::move(m2.value), m1.log + m2.log};
}
        
}
