
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/detail/macro_decltype_auto_return.hpp"
#include "picapau/oracle/with_log/connect.hpp"
#include "picapau/rs/values.hpp"

namespace picapau { namespace oracle { namespace with_log { namespace rs {

auto connect(std::string username, std::string password, std::string service)
PICAPAU_DECLTYPE_AUTO_RETURN
( picapau::rs::values(with_log::connect(std::move(username), std::move(password), std::move(service))) )
            
}}}}
