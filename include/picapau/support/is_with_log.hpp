
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <type_traits>

namespace picapau { 

struct with_log_base {};

template<typename T>
struct is_with_log : std::is_base_of<with_log_base, T> {};
    
}
