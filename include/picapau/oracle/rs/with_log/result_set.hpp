
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/oracle/core/query_result.hpp"
#include "picapau/oracle/core/with_log/for_each.hpp"
#include <utility>

namespace picapau { namespace oracle { namespace rs { namespace with_log {

template<typename QueryResult, typename Sink>
struct result_set_source
{
    using expected_tuple_t = typename QueryResult::ResultSet::expected_tuple_t;
    
    result_set_source(boost::expected<QueryResult, std::string> rs, Sink sink)
        : _rs(std::move(rs))
        , _sink(std::move(sink))
    {
        if(!_rs) _sink(expected_tuple_t(_rs.get_unexpected()));
        else
        {
            auto& rsink = _sink;
            oracle::core::with_log::for_each(_rs.value().rows,[&rsink](expected_tuple_t i){ rsink(i); });
        }
    }
    
    boost::expected<QueryResult, std::string> _rs;
    Sink _sink;
};
                
}}}}

namespace picapau { namespace rs { 
        
template<typename Error, typename Sink, typename...Columns>
inline auto operator>>=(boost::expected<oracle::core::query_result<Columns...>, Error> lhs, Sink&& sink)
PICAPAU_DECLTYPE_AUTO_RETURN
( oracle::rs::with_log::result_set_source<oracle::core::query_result<Columns...>, Sink>
  (std::move(lhs), std::forward<Sink>(sink)) )
    
}}
