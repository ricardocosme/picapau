
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/oracle/connect.hpp"
#include "picapau/oracle/with_log/session.hpp"

namespace picapau { namespace oracle { namespace with_log {

inline picapau::with_log<boost::expected<session, std::string>>
connect(std::string username, std::string password, std::string service)
{
    using with_log_t = picapau::with_log<boost::expected<session, std::string>>;
    
    std::string log
        (with_log_t::logln("connect", "username='" + username + "'" +
                           " service='" + service + "'"));
    
    auto res =  picapau::oracle::connect(std::move(username),
                                         std::move(password),
                                         std::move(service));

    if(res) log += with_log_t::logln("connect", "Connected with success");
    else
        log += with_log_t::logln("connect", "Connect failed") +
            with_log_t::logln("connect", "Error: " + res.get_unexpected().value());

    return {std::move(res), std::move(log)};
}
        
}}}
