#define BOOST_TEST_MODULE PICAPAU_ORACLE
#include <boost/test/unit_test.hpp>

#include <iostream>
#include "cfg.hpp"
#include "my_with_log.hpp"
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
        std::cout << t.cmdExec << std::endl;
        std::cout << t.log << std::endl;
        if(t.value.valid())
            std::cout << "Value: " << t.value.value() << std::endl;
        else
            std::cout << "Error: " << t.value.get_unexpected().value() << std::endl;
    }
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

struct AppendCmdExec {
    template<typename ES>
    ES operator()(ES& es)
    {
        es.cmdExec.reset(new CmdExecution);
        return es;
    }
};

template<typename WithLog>
inline std::pair<std::unique_ptr<CmdExecution>, WithLog>
withCmdExec(WithLog wlog)
{
    std::unique_ptr<CmdExecution> cmdExec(new CmdExecution);
    wlog.cmdExec = cmdExec.get();
    return std::make_pair(std::move(cmdExec), std::move(wlog));
}

template<typename Pair>
inline typename Pair::second_type
second(Pair&& p)
{ return std::move(p.second); }

struct PassOnlySession {
    template<typename Pair>
    typename Pair::second_type& operator()(Pair& p)
    { return p.second; }
};

BOOST_AUTO_TEST_CASE(SuccessfulSelect)
{
    rs::values(withCmdExec(db::connect(username, password, service)))
        >>= rs::transform(PassOnlySession{})
        >>= db::rs::query<int>("SELECT poco_cd_poco FROM poco")
        >>= rs::join()
        >>= Print{};
}
