
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE PICAPAU_ORACLE
#include <boost/test/unit_test.hpp>
#include <iostream>
#include "cfg.hpp"
#include "picapau/oracle/with_log/connect.hpp"

namespace db = picapau::oracle::with_log;

BOOST_AUTO_TEST_CASE(SuccessfulConnect)
{
    auto s = db::connect(username, password, service);
    BOOST_TEST(!s.log.empty());
    std::cout << s.log << std::endl;
    BOOST_TEST(s.value.valid());
}

BOOST_AUTO_TEST_CASE(InvalidUserConnect)
{
    auto s = db::connect("<invalid>", password, service);
    BOOST_TEST(!s.log.empty());
    std::cout << s.log << std::endl;
    BOOST_TEST(!s.value.valid());
}

BOOST_AUTO_TEST_CASE(InvalidPasswordConnect)
{
    auto s = db::connect(username, "<invalid>", service);
    BOOST_TEST(!s.log.empty());
    std::cout << s.log << std::endl;
    BOOST_TEST(!s.value.valid());
}

BOOST_AUTO_TEST_CASE(InvalidServiceConnect)
{
    auto s = db::connect(username, password, "<invalid>");
    BOOST_TEST(!s.log.empty());
    std::cout << s.log << std::endl;
    BOOST_TEST(!s.value.valid());
}
