
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/detail/macro_decltype_auto_return.hpp"
#include <utility>

namespace picapau { namespace rs { 

template<typename Sink>
struct sink_joined
{
    template<typename Stream>
    void operator()(Stream&& stream)
    { std::forward<Stream>(stream) >>= _sink; }
    
    Sink _sink;
};
                
struct join{};
            
template<typename Sink>
inline auto operator>>=(join, Sink&& sink)
PICAPAU_DECLTYPE_AUTO_RETURN
( sink_joined<Sink>{std::forward<Sink>(sink)} )
    
}}
