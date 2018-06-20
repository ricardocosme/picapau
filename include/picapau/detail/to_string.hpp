
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>
#include <utility>

namespace picapau { namespace detail {

template<typename T>        
std::string to_string(T&& s)
{ return std::to_string(std::forward<T>(s)); }
        
std::string to_string(std::string s)
{ return std::move(s); }

}}
