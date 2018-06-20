
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/oracle/session.hpp"
#include <string>

namespace picapau { namespace oracle { namespace detail {

inline session
connect(std::string username, std::string password, std::string service)
{
    auto env = ::oracle::occi::Environment::createEnvironment();
    auto conn = env->createConnection(username, password, service);
    return session(*env, *conn);
}

}}}
