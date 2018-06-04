
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/detail/macro_decltype_auto_return.hpp"
#include <utility>

namespace picapau { namespace rs { 

template<typename F>
struct transform_wrapper
{
    transform_wrapper(F f)
        : _f(std::move(f))
    {}
    
    template<typename T>
    auto operator()(T&& o) const
        -> decltype(std::declval<F>()(std::declval<T>()))
    { return _f(std::forward<T>(o)); }
    
    F _f;
};
    
template<typename Transform, typename Sink>
struct sink_transformed
{
    template<typename Input>
    void operator()(Input&& in) const
    { _sink(_transform(std::forward<Input>(in))); }
    
    Transform _transform;
    Sink _sink;
};
            
template<typename F>
inline auto transform(F&& f)
PICAPAU_DECLTYPE_AUTO_RETURN
( transform_wrapper<F>(std::forward<F>(f)) )
    
template<typename F, typename Sink>
inline auto operator>>=(transform_wrapper<F> lhs, Sink&& sink)
PICAPAU_DECLTYPE_AUTO_RETURN
( sink_transformed<transform_wrapper<F>, Sink>
  {std::move(lhs), std::forward<Sink>(sink)} )

}}
    
