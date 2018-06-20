
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/detail/macro_decltype_auto_return.hpp"
#include "picapau/rs/transform.hpp"
#include <utility>

namespace picapau { namespace rs { 

template<typename F>        
struct mbind_wrapper : transform_wrapper<F>
{
    using base = transform_wrapper<F>;
    using base::base;
};
        
template<typename Transform, typename Sink>
struct sink_mbinded
{
    template<typename Input>
    void operator()(Input&& in)
    { _transform(std::forward<Input>(in)) >>= _sink; }
    
    Transform _transform;
    Sink _sink;
};
            
template<typename F>
inline auto mbind(F&& f)
PICAPAU_DECLTYPE_AUTO_RETURN
( mbind_wrapper<F>(std::forward<F>(f)) )
    
template<typename F, typename Sink>
inline auto operator>>=(mbind_wrapper<F> lhs, Sink&& sink)
PICAPAU_DECLTYPE_AUTO_RETURN
( sink_mbinded<mbind_wrapper<F>, Sink>
  {std::move(lhs), std::forward<Sink>(sink)} )

}}
    
