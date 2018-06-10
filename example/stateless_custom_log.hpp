#pragma once

#include <iostream>

namespace picapau { namespace with_log {

struct my_log
{
    template<typename Exp>
    Exp operator()(Exp&& exp, std::string log)
    {
        std::cout << "[custom log] " << std::endl << log;
        return std::forward<Exp>(exp);
    }
};

using handle_log = my_log;

}}
