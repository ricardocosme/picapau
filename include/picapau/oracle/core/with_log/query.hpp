
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/oracle/core/query.hpp"
#include "picapau/support/log.hpp"
#include <string>
#include <type_traits>

namespace picapau { namespace oracle { namespace core { namespace with_log {

template<typename... Columns>
using query_result = oracle::core::query_result<Columns...>;
            
//TODO:std::error_code instead of std::string
template<typename... Columns, typename F = handle_log>            
inline typename std::result_of<
    F(boost::expected<query_result<Columns...>, std::string>, std::string)>::type
exec_query(session s, std::string squery,
           F&& f = handle_log())
{
    std::string log(logln("query", "statement='" + squery + "'"));
    
    auto res = oracle::core::exec_query<Columns...>(std::move(s), std::move(squery));

    if(res) log += logln("query", "Executed with success");
    else
        log += logln("query", "Query failed") +
            logln("query", "Error: " + res.get_unexpected().value());

    return f(std::move(res), std::move(log));
}
        
//Lazy version of exec_query as a partial function that handles an
//expected session
template<typename... Columns>        
struct query
{
    query(std::string squery)
        : _squery(std::move(squery))
    {}
    
    //TODO:std::error_code instead of std::string
    boost::expected<query_result<Columns...>, std::string>
    operator()(boost::expected<session, std::string> s) const
    {
        return s.bind([this](session s)
                      { return oracle::core::with_log::exec_query<Columns...>(std::move(s), _squery); });
    }

    std::string _squery;
};
            
}}}}
