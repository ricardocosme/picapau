
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/support/with_log.hpp"
#include "picapau/oracle/with_log/result_set.hpp"
// #include <range/v3/range_for.hpp>
#include <range/v3/all.hpp>
#include <utility>

namespace picapau { namespace oracle { namespace with_log { namespace rs {

template<typename Sink>            
struct F
{
    template<typename T>
    void operator()(T&& t) const
    { sink(std::forward<T>(t)); }
    Sink& sink;
};
            
template<typename ResultSet, typename Sink>
struct result_set_source
{
    result_set_source(ResultSet rs, Sink sink)
    {
        if(rs.value.valid())
        {            
            auto& tuples = rs.value.value();
            auto it = ranges::begin(tuples);
            if(it != ranges::end(tuples))
            {
                auto fst = *it;
                decltype(fst) transformed_fst(std::move(fst.value),
                                              rs.log + fst.log);
                sink(std::forward<decltype(*it)>(std::move(transformed_fst)));
                ++it;
            }
            for(; it != ranges::end(tuples); ++it)
            { sink(std::forward<decltype(*it)>(*it)); }
        }
        else
        {
            sink(typename ResultSet::value_type::value_type::expected_tuple_t
                 (rs.value.get_unexpected(), rs.log));
        }
    }
};
                
}}}}

namespace picapau { namespace rs {
        
template<typename Error, typename Sink, typename...Columns>
inline auto operator>>=(picapau::with_log<
                        boost::expected<oracle::with_log::result_set<Columns...>, Error>
                        > lhs,
                        Sink&& sink)
PICAPAU_DECLTYPE_AUTO_RETURN
    ( oracle::with_log::rs::result_set_source<picapau::with_log<boost::expected<oracle::with_log::result_set<Columns...>, Error>>, Sink> (std::move(lhs), std::forward<Sink>(sink)) )
    
}}
