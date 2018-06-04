
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/detail/make_expected_from_call.hpp"
#include "picapau/oracle/core/detail/query.hpp"
#include "picapau/oracle/core/query_result.hpp"
#include <boost/expected/expected.hpp>
#include <string>
#include <utility>

namespace picapau { namespace oracle { namespace core {
        
//TODO:std::error_code instead of std::string
template<typename... Columns>
inline boost::expected<query_result<Columns...>, std::string>
exec_query(session s, std::string squery)
{
    return picapau::detail::make_expected_from_call
        (detail::query_wrapped<Columns...>{std::move(s), std::move(squery)});
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
                      { return exec_query<Columns...>(std::move(s), _squery); });
    }

    std::string _squery;
};
        
}}}
