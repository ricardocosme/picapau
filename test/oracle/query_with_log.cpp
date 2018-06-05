#define BOOST_TEST_MODULE PICAPAU_ORACLE
#include <boost/test/unit_test.hpp>

#include "cfg.hpp"
#include "picapau/utils/log_to_cout.hpp"
#include "picapau/oracle/core/with_log/connect.hpp"
#include "picapau/oracle/core/with_log/for_each.hpp"
#include "picapau/oracle/core/with_log/query.hpp"
#include <boost/fusion/include/at_c.hpp>
#include <vector>

namespace db = picapau::oracle::core::with_log;
using namespace boost::fusion;

BOOST_AUTO_TEST_CASE(SuccessfulSelect)
{
    std::vector<int> v;
    db::connect(username, password, service)
        .bind([&](db::session s)
              { return db::exec_query<int>(std::move(s), "SELECT poco_cd_poco FROM poco"); })
        .map([&](db::query_result<int> r)
             {
                 db::for_each(r.rows, [&](boost::expected<vector1<int>, std::string> row)
                               {
                                   BOOST_TEST(row.valid());
                                   v.push_back(at_c<0>(row.value()));
                               });
             });
    
    BOOST_TEST(v.size() == 2);
}
