
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/detail/make_expected_from_call.hpp"
#include "picapau/oracle/detail/query.hpp"
#include "picapau/oracle/result_set.hpp"
#include <string>
#include <utility>

namespace picapau { namespace oracle { 
        
template<typename... Columns>
inline boost::expected<result_set<Columns...>, std::string>
exec_query(const session& s, std::string squery)
{
    return picapau::detail::make_expected_from_call
        ([&] { return detail::exec_query<result_set<Columns...>, Columns...>(s, std::move(squery)); });
}
        
}}
