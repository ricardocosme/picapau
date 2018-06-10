
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include "stateless_custom_log.hpp"
#include "picapau/with_log/oracle/rs/connect.hpp"
#include "picapau/with_log/oracle/rs/query.hpp"
#include <boost/fusion/include/io.hpp>
#include <iostream>

namespace sql = picapau::with_log;
namespace db = sql::oracle::core;
namespace rs = sql::oracle::rs;

using namespace boost::fusion;

int main()
{
    rs::connect("system", "system", "localhost:1521/XE")
        >>= rs::query<int>("SELECT poco_cd_poco FROM poco")
        >>= [](boost::expected<vector1<int>, std::string> row)
        {
            row ? std::cout << row.value() << std::endl
                : std::cout << row.get_unexpected().value() << std::endl;
        };
}
