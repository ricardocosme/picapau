
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <utility>
#include <range/v3/range_for.hpp>
#include <range/v3/range_concepts.hpp>

namespace picapau { namespace rs { 

template<typename Rng, typename Sink>
struct rng_impl
{
    rng_impl(Rng rng, Sink sink)
    {
        RANGES_FOR(auto&& e, rng)
        { sink(std::forward<decltype(e)>(e)); }
    }
};

template<typename T>
std::initializer_list<T> stream(std::initializer_list<T> il)
{ return il; }
        
template<typename Rng, typename Sink>
inline typename std::enable_if<
    ranges::Range<Rng>::value, rng_impl<Rng, Sink>
>::type
operator>>=(Rng lhs, Sink&& sink)
{ return rng_impl<Rng, Sink>(std::move(lhs), std::forward<Sink>(sink)); }

}}
    
