
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE PICAPAU_ORACLE
#include <boost/test/unit_test.hpp>

#include "cfg.hpp"
#include "picapau/oracle/rs/connect.hpp"
#include "picapau/oracle/rs/query.hpp"
#include <boost/fusion/include/at_c.hpp>
#include <vector>

namespace db = picapau::oracle::core;
namespace rs = picapau::oracle::rs;
using namespace boost::fusion;

BOOST_AUTO_TEST_CASE(SuccessfulQuery)
{
    std::vector<int> v;
    
    rs::connect(username, password, service)
        >>= rs::query<int>("SELECT poco_cd_poco FROM poco")
        >>= [&](boost::expected<vector1<int>, std::string> row)
        {
            BOOST_TEST(row.valid());
            v.push_back(at_c<0>(row.value()));
        };
    
    BOOST_TEST(v.size() == 2);
}

BOOST_AUTO_TEST_CASE(FailedSelect)
{
    rs::connect(username, password, service)
        >>= rs::query<int>("<invalid>")
        >>= [](boost::expected<vector1<int>, std::string> row)
        {
            BOOST_TEST(!row.valid());
            BOOST_TEST(row.get_unexpected().value() != "");
        };
}

BOOST_AUTO_TEST_CASE(FailedConnectWithSelect)
{
    rs::connect("<invalid>", password, service)
        >>= rs::query<int>("SELECT poco_cd_poco FROM poco")
        >>= [](boost::expected<vector1<int>, std::string> row)
        {
            BOOST_TEST(!row.valid());
            BOOST_TEST(row.get_unexpected().value() != "");
        };
}
