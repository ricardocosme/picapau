
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include "picapau/with_log/oracle/rs/connect.hpp"
#include "picapau/with_log/oracle/rs/query.hpp"
#include <boost/fusion/include/io.hpp>
#include <iostream>
#include <utility>

namespace picapau { namespace with_log {
        
struct stats_log
{
    stats_log(std::string operation)
        : _operation(std::move(operation))
    {}
    
    template<typename Exp>
    Exp operator()(Exp&& exp, std::string log)
    {
        std::cout << _operation << " of number #" << ++_count
                  << std::endl << log << std::flush;
        return std::forward<Exp>(exp);
    }
    
    std::string _operation;
    std::size_t _count{0};
};
        
}}

using stats_log = picapau::with_log::stats_log;

namespace sql = picapau::with_log;
namespace db = sql::oracle::core;
namespace rs = sql::oracle::rs;

using namespace boost::fusion;

stats_log connect_log("connect");
stats_log query_log("query");

int main()
{
    rs::connect("system", "system", "localhost:1521/XE", connect_log)
        >>= rs::query<int>("SELECT poco_cd_poco FROM poco", query_log)
        >>= [](boost::expected<vector1<int>, std::string> row)
        {
            row ? std::cout << row.value() << std::endl
                : std::cout << row.get_unexpected().value() << std::endl;
        };
}
