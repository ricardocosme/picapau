
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/oracle/core/with_log/fetch.hpp"
#include "picapau/oracle/core/for_each.hpp"
#include "picapau/oracle/core/query_result.hpp"
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/fusion/include/vector.hpp>

namespace picapau { namespace oracle { namespace core { namespace with_log {

template<typename Tuple, typename Log>
struct column_fetch
{
    template<typename Column>
    void operator()(Column& column) const
    {
        oracle::core::fetch_column(rs, column, idx, tuple, fetch<Column, Log>{log});
    }
    ::oracle::occi::ResultSet& rs;
    std::size_t& idx;
    Tuple& tuple;
    Log& log;
};
                
template<typename ResultSet, typename F, typename Log = handle_log>
inline typename std::result_of<Log(boost::expected<void, std::string>, std::string)>::type
for_each(ResultSet& rs, F&& f, Log&& hlog = handle_log())
{
    using response_t = typename ResultSet::expected_tuple_t;
    std::size_t n_tuples{0};
    while(rs->next())
    {        
        oracle::core::fetch_tuple<ResultSet, F, column_fetch<response_t, Log>>(rs, f, hlog);
        ++n_tuples;
    }
    
    std::string log(logln("for_each", "(" + std::to_string(n_tuples) +
                          ") tuples fetched"));
    
    return hlog(boost::expected<void, std::string>{}, std::move(log));
}
        
}}}}
