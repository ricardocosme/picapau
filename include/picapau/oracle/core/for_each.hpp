
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

template<typename Tuple>
struct column_fetch
{
    template<typename Column>
    void operator()(Column& column) const
    {
        if(!tuple) return;
        
        auto fetched = fetch<Column>(rs, idx);
        if(fetched) column = fetched.value();
        else tuple = fetched.get_unexpected();
    }
    ::oracle::occi::ResultSet& rs;
    std::size_t& idx;
    Tuple& tuple;   
};
        
template<typename ResultSet, typename F>
inline void for_each(ResultSet& rs, F&& f)
{    
    std::size_t idx{1};
    while(rs->next())
    {
        using tuple_t = typename ResultSet::tuple_t;
        using response_t = typename ResultSet::expected_tuple_t;
        response_t tuple(tuple_t{});
        boost::fusion::for_each(tuple.value(),
                                column_fetch<response_t>{*rs, idx, tuple});
        f(std::move(tuple));
    }
}
        
}}}
