
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/detail/make_expected_from_call.hpp"
#include "picapau/oracle/statement.hpp"
#include <boost/expected/expected.hpp>
#include <string>
#include <utility>

namespace picapau { namespace oracle { namespace detail {

template<typename ResultSet, typename... Columns>            
inline ResultSet
exec_query(const session& s, std::string squery)
{
    auto stmt = statement(*s.conn(), *s.conn()->createStatement());
    auto ors = stmt->executeQuery(squery);
    return ResultSet(std::move(stmt), *ors);
}
            
}}}
