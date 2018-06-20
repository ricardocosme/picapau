#define BOOST_TEST_MODULE PICAPAU_ORACLE
#include <boost/test/unit_test.hpp>

#include "cfg.hpp"
#include "picapau/oracle/connect.hpp"
#include "picapau/oracle/query.hpp"
#include <range/v3/range_for.hpp>
#include <vector>

namespace db = picapau::oracle;
using namespace boost::fusion;

BOOST_AUTO_TEST_CASE(SuccessfulSelect)
{
    std::vector<vector<int>> v;
    auto conn = db::connect(username, password, service);
    conn.bind([](const db::session& s)
              { return db::exec_query<int>(s, "SELECT poco_cd_poco FROM poco"); })
        .map([&](db::result_set<int> rs)
             {
                 RANGES_FOR(auto&& tuple, rs)
                 {
                     BOOST_TEST(tuple.valid());
                     v.push_back(tuple.value());
                 }
             });    
    BOOST_TEST(v.size() == 2);
}
