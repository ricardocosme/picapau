#define BOOST_TEST_MODULE PICAPAU_ORACLE
#include <boost/test/unit_test.hpp>

#include "cfg.hpp"
#include "picapau/support/lift_with_log.hpp"
#include "picapau/oracle/with_log/connect.hpp"
#include "picapau/oracle/with_log/query.hpp"
#include <range/v3/range_for.hpp>
#include <vector>
#include <iostream>
namespace db = picapau::oracle::with_log;
using namespace boost::fusion;

BOOST_AUTO_TEST_CASE(SuccessfulSelect)
{
    std::vector<vector<int, std::string>> v;
    auto conn = db::connect(username, password, service);
    auto m = mbind(conn, picapau::lift_with_log
                   ([](const db::session& s)
                    { return db::exec_query<int, std::string>(s, "SELECT poco_cd_poco, poco_nm_completo FROM poco"); }));
    std::cout << m.log;
    m.value.map([&](db::result_set<int, std::string> rs)
                {
                    RANGES_FOR(auto&& tuple, rs)
                    {
                        BOOST_TEST(tuple.value.valid());
                        std::cout << tuple.log;
                        v.push_back(tuple.value.value());
                    }
                });
    BOOST_TEST(v.size() == 2);
}
