
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/oracle/core/fetch.hpp"
#include "picapau/oracle/core/query_result.hpp"
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/fusion/include/vector.hpp>

namespace picapau { namespace oracle { namespace core {

template<typename Tuple, typename Column, typename Fetch>            
inline void fetch_column(::oracle::occi::ResultSet& rs,
                         Column& column,
                         std::size_t& idx,
                         Tuple& tuple,
                         Fetch&& f)
{
        if(!tuple) return;
        
        auto fetched = f(rs, idx++);
        if(fetched) column = fetched.value();
        else tuple = fetched.get_unexpected();
}
            
template<typename Tuple>
struct column_fetch
{
    template<typename Column>
    void operator()(Column& column) const
    {
        fetch_column(rs, column, idx, tuple, fetch<Column>);
    }
    ::oracle::occi::ResultSet& rs;
    std::size_t& idx;
    Tuple& tuple;   
};

template<typename ResultSet, typename F, typename ColumnFetch>            
inline void fetch_tuple(ResultSet& rs, F& f)
{
    std::size_t idx{1};
    using tuple_t = typename ResultSet::tuple_t;
    using response_t = typename ResultSet::expected_tuple_t;
    response_t tuple(tuple_t{});
    boost::fusion::for_each(tuple.value(),
                            ColumnFetch{*rs, idx, tuple});
    f(std::move(tuple));
}
            
template<typename ResultSet, typename F>
inline void for_each(ResultSet& rs, F&& f)
{    
    using response_t = typename ResultSet::expected_tuple_t;
    while(rs->next())
        fetch_tuple<ResultSet, F, column_fetch<response_t>>(rs, f);
}
        
}}}
