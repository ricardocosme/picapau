
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/detail/macro_decltype_auto_return.hpp"
#include "picapau/with_log/oracle/core/query.hpp"
#include "picapau/with_log/oracle/rs/result_set.hpp"
#include "picapau/rs/mbind.hpp"
#include <string>

namespace picapau { namespace with_log {namespace oracle { namespace rs { 

template<typename... Columns, typename HandleLog = handle_log>
inline auto query(std::string squery, HandleLog&& hlog = handle_log())
PICAPAU_DECLTYPE_AUTO_RETURN
    ( picapau::rs::mbind(core::query<HandleLog, Columns...>(std::move(squery), std::forward<HandleLog>(hlog))) )

}}}}
