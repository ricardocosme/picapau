
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/detail/make_expected_from_call.hpp"
#include "picapau/oracle/core/query_result.hpp"
#include <boost/expected/expected.hpp>
#include <string>
#include <utility>

namespace picapau { namespace oracle { namespace core { namespace detail {

template<typename... Columns>            
inline query_result<Columns...>
exec_query(session s, std::string squery)
{
    auto stmt = statement(*s.conn(), *s.conn()->createStatement());
    auto rs = result_set<Columns...>(*stmt, *stmt->executeQuery(squery));
    return query_result<Columns...>{std::move(s), std::move(stmt), std::move(rs)};
}

template<typename... Columns>            
struct query_wrapped
{
    query_result<Columns...> operator()()
    { return detail::exec_query<Columns...>(std::move(s), std::move(squery)); }
    
    session s;
    std::string squery;
};
    
}}}}
