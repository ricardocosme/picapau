
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/detail/make_expected_from_call.hpp"
#include "picapau/oracle/detail/query.hpp"

#include "picapau/oracle/with_log/result_set.hpp"

namespace picapau { namespace oracle { namespace with_log {
        
template<typename... Columns>
inline picapau::with_log<boost::expected<result_set<Columns...>, std::string>>
exec_query(const session& s, std::string squery)
{
    using with_log_t = picapau::with_log<boost::expected<result_set<Columns...>,
                                               std::string>>;
    
    std::string log(with_log_t::logln("query", "statement='" + squery + "'"));

    auto res = picapau::detail::make_expected_from_call
        ([&] { return picapau::oracle::detail::exec_query
                <result_set<Columns...>, Columns...>(s, std::move(squery)); });
    
    if(res) log += with_log_t::logln("query", "Executed with success");
    else
        log += with_log_t::logln("query", "Query failed") +
            with_log_t::logln("query", "Error: " + res.get_unexpected().value());

    return {std::move(res), std::move(log)};
}
        
}}}
