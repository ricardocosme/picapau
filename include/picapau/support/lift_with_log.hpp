
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/detail/macro_decltype_auto_return.hpp"
#include <type_traits>
#include <utility>

namespace picapau {
    
template<typename F>        
struct LiftWithLog
{
    template<typename Exp>
    typename std::result_of<F(typename std::decay<Exp>::type::value_type&)>::type
    operator()(Exp&& exp)
    {
        if(exp.valid())
            return f(std::forward<Exp>(exp).value());
        return {exp.get_unexpected(), ""};
    }
    F f;
};
        
template<typename F>
inline auto lift_with_log(F&& f)
PICAPAU_DECLTYPE_AUTO_RETURN
( LiftWithLog<F>{std::forward<F>(f)} )

}
