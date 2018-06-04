
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE PICAPAU_ORACLE
#include <boost/test/unit_test.hpp>

#include "cfg.hpp"
#include "picapau/oracle/rs/connect.hpp"
#include <vector>

namespace db = picapau::oracle::core;
namespace rs = picapau::oracle::rs;

BOOST_AUTO_TEST_CASE(SuccessfulConnect)
{
    rs::connect(username, password, service)
        >>= [&](boost::expected<db::session, std::string> s)
        { BOOST_TEST(s.valid()); };
}

BOOST_AUTO_TEST_CASE(FailedConnect)
{
    rs::connect("<invalid>", password, service)
        >>= [&](boost::expected<db::session, std::string> s)
        {
            BOOST_TEST(!s.valid());
            BOOST_TEST(s.get_unexpected().value() != "");
        };
}
