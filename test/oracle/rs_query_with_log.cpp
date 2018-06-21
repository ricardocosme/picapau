#define BOOST_TEST_MODULE PICAPAU_ORACLE
#include <boost/test/unit_test.hpp>

#include "cfg.hpp"
#include "picapau/support/with_log.hpp"
#include "picapau/oracle/with_log/rs/connect.hpp"
#include "picapau/oracle/with_log/rs/fetch.hpp"
#include "picapau/oracle/with_log/rs/query.hpp"
#include <boost/fusion/include/io.hpp>
#include <iostream>

namespace db = picapau::oracle::with_log;
namespace rs = picapau::rs;
using namespace boost::fusion;

struct Print {
    template<typename T>
    void operator()(T&& t) const
    { std::cout << t << std::endl; }
};

struct HandleLog {
    void operator()(std::string log)
    { std::cout << log << std::endl; }
};

struct HandleError {
    void operator()(std::string error)
    { std::cout << error << std::endl; }
};

BOOST_AUTO_TEST_CASE(SuccessfulSelect)
{
    db::rs::connect(username, password, service)
        >>= db::rs::query<int>("SELECT poco_cd_poco FROM poco")
        >>= db::rs::fetch(HandleLog{}, HandleError{})
        >>= Print{};
}
