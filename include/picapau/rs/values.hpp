
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/detail/macro_decltype_auto_return.hpp"
#include <utility>
#include <vector>

namespace picapau { namespace rs { 

template<typename T>
struct values_wrapper
{
    using values_t = std::array<T, 1>;
    values_t values;
};
    
template<typename Values, typename Sink>
struct values_impl
{
    values_impl(Values values, Sink sink)
    {
        for(auto& e : values)
            sink(e);
    }
};
            
template<typename Arg0>
inline auto values(Arg0&& arg0)
PICAPAU_DECLTYPE_AUTO_RETURN
( values_wrapper<Arg0>{std::forward<Arg0>(arg0)} )
    
template<typename T, typename Sink>
inline auto operator>>=(values_wrapper<T> lhs, Sink&& sink)
PICAPAU_DECLTYPE_AUTO_RETURN
( values_impl<typename values_wrapper<T>::values_t, Sink>
 (std::move(lhs.values), std::forward<Sink>(sink)) )

}}
    
