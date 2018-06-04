
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <iostream>
#include <string>

namespace picapau { 

struct log_to_cout
{
    template<typename Exp>
    Exp operator()(Exp&& exp, std::string log)
    {
        std::cout << log << std::flush;
        return std::forward<Exp>(exp);
    }
};

using handle_log = log_to_cout;
            
}
