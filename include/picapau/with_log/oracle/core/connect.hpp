
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/utils/log_to_cout.hpp"
#include "picapau/oracle/core/connect.hpp"
#include "picapau/support/log.hpp"
#include <string>
#include <type_traits>

namespace picapau { namespace with_log { namespace oracle { namespace core { 

using session = picapau::oracle::core::session;
            
//TODO:std::error_code instead of std::string
template<typename F = handle_log>            
inline typename std::result_of<
    F(boost::expected<session, std::string>, std::string)>::type
connect(std::string username, std::string password, std::string service,
        F&& f = handle_log())
{
    std::string log
        (logln("connect", "username='" + username + "'" +
               " password='" + password + "'" +
               " service='" + service + "'"));
    
    auto res =  picapau::oracle::core::connect(std::move(username),
                                               std::move(password),
                                               std::move(service));

    if(res) log += logln("connect", "Connected with success");
    else
        log += logln("connect", "Connect failed") +
            logln("connect", "Error: " + res.get_unexpected().value());

    return f(std::move(res), std::move(log));
}
        
}}}}
