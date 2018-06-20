#define BOOST_TEST_MODULE PICAPAU_ORACLE
#include <boost/test/unit_test.hpp>

#include <iostream>
#include "cfg.hpp"
#include "picapau/oracle/with_log/rs/connect.hpp"
#include "picapau/oracle/with_log/rs/result_set.hpp"
#include "picapau/oracle/with_log/rs/query.hpp"
// #include "picapau/rs/values.hpp"
#include "picapau/rs/range.hpp"
#include "picapau/rs/filter.hpp"
#include "picapau/rs/join.hpp"
#include "picapau/rs/transform.hpp"
#include <boost/fusion/include/mpl.hpp>
#include <boost/fusion/include/io.hpp>
#include <range/v3/view/transform.hpp>
#include <vector>
#include <memory>

namespace db = picapau::oracle::with_log;
namespace rs = picapau::rs;
using namespace boost::fusion;

struct Print {
    template<typename T>
    void operator()(T&& t) const
    {
        std::cout << t.second << std::endl;
        std::cout << t.first.log;
        if(t.first.value.valid())
            std::cout << t.first.value.value() << std::endl;
        else
            std::cout << t.first.value.get_unexpected().value() << std::endl;
    }
};

struct CmdExecution{
    CmdExecution(){ std::cout << "CmdExecution ctor" << std::endl; }
    ~CmdExecution(){ std::cout << "CmdExecution dtor" << std::endl; }
};

struct AppendImpl
{
    CmdExecution* cmd_ptr;
    
    template<typename T>
    auto operator()(T t) const
    PICAPAU_DECLTYPE_AUTO_RETURN
    ( std::make_pair(t, cmd_ptr) )
};

struct AppendCmdExecution
{
    std::unique_ptr<CmdExecution> cmdExec;

    AppendCmdExecution()
        : cmdExec(new CmdExecution) {}
    
    template<typename Rs>
    auto operator()(const Rs& rs) 
    PICAPAU_DECLTYPE_AUTO_RETURN
    ( rs.value() | ranges::view::transform(AppendImpl{cmdExec.get()}) )
};

struct HasResultSet {
    template<typename ExpResultSet>
    bool operator()(ExpResultSet& e)
    { return e.valid(); }
};

BOOST_AUTO_TEST_CASE(SuccessfulSelect)
{
    // std::vector<vector<int>> v;
    db::rs::connect(username, password, service)
        >>= db::rs::query<int>("SELECT poco_cd_poco FROM poco")
        >>= rs::transform([](picapau::with_log<boost::expected<db::result_set<int>, std::string>> rs)
                          {
                              //Handling log
                              std::cout << rs.log;
                              //Handling error
                              if(!rs.value.valid())
                                  std::cout << rs.value.get_unexpected().value();
                              return std::move(rs.value);
                          })
        >>= rs::filter(HasResultSet{})
        >>= rs::transform(AppendCmdExecution{})
        >>= rs::join()
        >>= Print{};
        // >>= [](picapau::with_log<boost::expected<db::result_set<int>, std::string>> rs)
        //     { std::cout << rs.log << std::endl; };
    // BOOST_TEST(v.size() == 2);
}
