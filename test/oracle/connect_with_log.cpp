
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE PICAPAU_ORACLE
#include <boost/test/unit_test.hpp>

#include "cfg.hpp"
#include "picapau/utils/log_to_cout.hpp"
#include "picapau/with_log/oracle/core/connect.hpp"

namespace db = picapau::with_log::oracle::core;

BOOST_AUTO_TEST_CASE(SuccessfulConnect)
{
    auto s = db::connect(username, password, service);
    BOOST_TEST(s.valid());
}

BOOST_AUTO_TEST_CASE(InvalidUserConnect)
{
    auto res = db::connect("<invalid>", password, service);
    BOOST_TEST(!res.valid());
}

BOOST_AUTO_TEST_CASE(InvalidPasswordConnect)
{
    auto res = db::connect(username, "<invalid>", service);
    BOOST_TEST(!res.valid());
}

BOOST_AUTO_TEST_CASE(InvalidServiceConnect)
{
    auto res = db::connect(username, password, "<invalid>");
    BOOST_TEST(!res.valid());
}
