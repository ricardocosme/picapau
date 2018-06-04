#pragma once

#include <iostream>

struct my_log
{
    template<typename Exp>
    Exp operator()(Exp&& exp, std::string log)
    {
        std::cout << "[custom log]" << std::endl;
        std::cout << log << std::endl;
        return std::forward<Exp>(exp);
    }
};

using handle_log = my_log;

