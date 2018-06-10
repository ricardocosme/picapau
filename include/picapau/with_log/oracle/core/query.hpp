
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/oracle/core/query.hpp"
#include "picapau/support/log.hpp"
#include <string>
#include <type_traits>

namespace picapau { namespace with_log { namespace oracle { namespace core { 

template<typename... Columns>
using query_result = picapau::oracle::core::query_result<Columns...>;
            
//TODO:std::error_code instead of std::string
template<typename... Columns, typename F = handle_log>            
inline typename std::result_of<
    F(boost::expected<query_result<Columns...>, std::string>, std::string)>::type
exec_query(picapau::oracle::core::session s, std::string squery,
           F&& f = handle_log())
{
    std::string log(logln("query", "statement='" + squery + "'"));
    
    auto res = picapau::oracle::core::exec_query<Columns...>(std::move(s), std::move(squery));

    if(res) log += logln("query", "Executed with success");
    else
        log += logln("query", "Query failed") +
            logln("query", "Error: " + res.get_unexpected().value());

    return f(std::move(res), std::move(log));
}
        
//Lazy version of exec_query as a partial function that handles an
//expected session
template<typename HandleLog = handle_log, typename... Columns> 
struct query
{
    template<typename F>
    query(std::string squery, F&& hlog = handle_log())
        : _squery(std::move(squery))
        , _hlog(std::forward<F>(hlog))
    {}
    
    //TODO:std::error_code instead of std::string
    typename std::result_of<
    HandleLog(boost::expected<query_result<Columns...>, std::string>, std::string)>::type
    operator()(boost::expected<picapau::oracle::core::session, std::string> s) const
    {
        return s.bind([this](picapau::oracle::core::session s)
                      { return picapau::with_log::oracle::core::exec_query<Columns...>
                              (std::move(s), _squery, _hlog); });
    }

    std::string _squery;
    mutable HandleLog _hlog;
};
            
}}}}
