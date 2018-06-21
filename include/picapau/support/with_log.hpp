
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/support/is_with_log.hpp"

#include <string>
#include <type_traits>
#include <utility>

namespace picapau { 

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
        
template<typename T,
         typename F,
         typename Ret = typename std::result_of<
             typename std::decay<F>::type(T&)
         >::type
>
inline Ret mbind(with_log<T>& m1, F&& f)
{
    auto m2 = f(m1.value);
    return {std::move(m2.value), m1.log + m2.log};
}
        
}
