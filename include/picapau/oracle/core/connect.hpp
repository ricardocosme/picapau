
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/detail/make_expected_from_call.hpp"
#include "picapau/oracle/core/detail/connect.hpp"
#include "picapau/oracle/core/session.hpp"
#include <string>

namespace picapau { namespace oracle { namespace core {

//TODO:std::error_code instead of std::string
inline boost::expected<session, std::string>
connect(std::string username, std::string password, std::string service)
{
    return picapau::detail::make_expected_from_call
        ([&]
         {
            return detail::connect
                (std::move(username), std::move(password), std::move(service));
         });
}
        
}}}
