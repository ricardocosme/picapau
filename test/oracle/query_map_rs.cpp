#define BOOST_TEST_MODULE PICAPAU_ORACLE
#include <boost/test/unit_test.hpp>

#include "cfg.hpp"
#include "picapau/oracle/connect.hpp"
#include "picapau/oracle/query.hpp"
#include <vector>
#include <range/v3/all.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/mpl.hpp>

namespace db = picapau::oracle;

BOOST_AUTO_TEST_CASE(SuccessfulSelect)
{
    std::vector<boost::fusion::vector<int>> v;
    auto conn = db::connect(username, password, service);
    conn.bind([](const db::session& s)
              { return db::exec_query<int>(s, "SELECT poco_cd_poco FROM poco"); })
        .map([&](db::result_set<int> rs)
             {
                 auto rng = rs
                     | ranges::view::transform([](boost::expected<boost::fusion::vector<int>, std::string> tuple)
                                               { return std::make_pair(std::move(tuple), std::string{"hi"});});
                 RANGES_FOR(auto&& p, rng)
                 {
                     BOOST_TEST(p.first.valid());
                     v.push_back(p.first.value());
                 }
             });    
    BOOST_TEST(v.size() == 2);
}
