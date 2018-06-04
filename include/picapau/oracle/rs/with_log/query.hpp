
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/detail/macro_decltype_auto_return.hpp"
#include "picapau/oracle/core/with_log/query.hpp"
#include "picapau/oracle/rs/result_set.hpp"
#include "picapau/rs/mbind.hpp"
#include <string>

namespace picapau { namespace oracle { namespace rs { namespace with_log {

template<typename... Columns>
inline auto query(std::string squery)
PICAPAU_DECLTYPE_AUTO_RETURN
( picapau::rs::mbind(core::with_log::query<Columns...>(std::move(squery))) )

}}}}
