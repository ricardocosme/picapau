
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include "picapau/oracle/rs/connect.hpp"
#include "picapau/oracle/rs/query.hpp"
#include <boost/fusion/include/at_c.hpp>
#include <iostream>

namespace db = picapau::oracle::core;
namespace rs = picapau::oracle::rs;

using namespace boost::fusion;

int main()
{
    rs::connect("system", "system", "localhost:1521/XE")
        >>= rs::query<int>("SELECT poco_cd_poco FROM poco")
        >>= [](boost::expected<vector1<int>, std::string> row)
        {
            row ? std::cout << "Value: "
                            << at_c<0>(row.value()) << std::endl
                : std::cout << "Error: "
                            << row.get_unexpected().value() << std::endl;
                
        };
}
