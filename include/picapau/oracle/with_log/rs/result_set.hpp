
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/oracle/with_log/result_set.hpp"
#include <range/v3/begin_end.hpp>
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
        using expected_t = typename ResultSet::value_type;
        using value_type = typename expected_t::value_type;
        
        if(rs.value.valid())
        {            
            auto& tuples = rs.value.value();
            auto it = ranges::begin(tuples);
            if(it != ranges::end(tuples))
            {
                auto transformed_fst = picapau::mbind(rs, [&it](const expected_t&)
                                                          { return *it; });
                sink(std::forward<decltype(*it)>(std::move(transformed_fst)));
                ++it;
            }
            rs.log = "";
            for(; it != ranges::end(tuples); ++it)
                { sink(picapau::mbind(rs, [&it](const expected_t&)
                                          { return std::forward<decltype(*it)>(*it); })); }
        }
        else
        {
            sink(picapau::mbind(rs, [](const expected_t& e)
                                    {
                                        return typename value_type::expected_tuple_t
                                            (e.get_unexpected());
                                    }));
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
