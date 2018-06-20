
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

template<typename T>
struct my_with_log_t : picapau::with_log<T>
{
    using base = picapau::with_log<T>;
    using base::base;
    
    static std::string logln(std::string label, std::string msg)
    { return ">" + label + "< " + msg + "\n"; }
};

BOOST_AUTO_TEST_CASE(SuccessfulConnect)
{
    auto s = db::connect<my_with_log_t>(username, password, service);
    BOOST_TEST(!s.log.empty());
    std::cout << s.log << std::endl;
    BOOST_TEST(s.value.valid());
}

