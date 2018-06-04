
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/oracle/core/result_set.hpp"
#include "picapau/oracle/core/statement.hpp"
#include "picapau/oracle/core/session.hpp"
#include <string>

namespace picapau { namespace oracle { namespace core {

template<typename... Columns>        
struct query_result
{
    using ResultSet = result_set<Columns...>;
    
    session msession;
    statement stmt;
    result_set<Columns...> rows;
};
        
}}}
