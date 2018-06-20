
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/oracle/for_each.hpp"
#include "picapau/oracle/result_set.hpp"
#include "picapau/oracle/with_log/detail/fetch.hpp"
#include "picapau/support/with_log.hpp"
#include <boost/fusion/include/for_each.hpp>

namespace picapau { namespace oracle { namespace with_log {

template<typename Tuple>
struct column_fetch
{
    template<typename Column>
    void operator()(Column& column) const
    {
        if(!tuple.value) return;
        
        auto fetched = detail::fetch<Column>(rs, idx++);
        tuple.log += fetched.log;
        if(fetched.value) column = fetched.value.value();
        else tuple.value = fetched.value.get_unexpected();
    }
    ::oracle::occi::ResultSet& rs;
    std::size_t& idx;
    Tuple& tuple;   
};
        
template<typename ResultSet_, typename F>
inline void for_each(ResultSet_&& rs, F&& f)
{
    using ResultSet = typename std::decay<ResultSet_>::type;
    using response_t = picapau::with_log<typename ResultSet::expected_tuple_t>;
    while(rs->next())
    {
        std::size_t idx{1};
        response_t tuple{typename ResultSet::tuple_t{}};
        boost::fusion::for_each(tuple.value.value(), column_fetch<response_t>{*rs, idx, tuple});
        f(std::move(tuple));
    }
}
        
template<typename... Columns, typename T, typename F>
inline void for_each(map_result_set<result_set<Columns...>, T>& rs, F&& f)
{
    using ResultSet = typename map_result_set<result_set<Columns...>, T>::type;
    using response_t = picapau::with_log<typename ResultSet::expected_tuple_t>;
    while(rs.rs->next())
    {
        std::size_t idx{1};
        response_t tuple{typename ResultSet::tuple_t{}, ""};
        boost::fusion::for_each(tuple.value.value(), column_fetch<response_t>{*(rs.rs), idx, tuple});
        f(rs.f(std::move(tuple)));
    }
}
            
}}}
