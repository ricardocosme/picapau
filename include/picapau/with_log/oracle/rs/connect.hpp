
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/detail/macro_decltype_auto_return.hpp"
#include "picapau/with_log/oracle/core/connect.hpp"
#include <string>

namespace picapau { namespace with_log { namespace oracle { namespace rs { 

template<typename HandleLog>                
struct connect_wrapper
{
    std::string username, password, service;
    HandleLog hlog;
};
                
template<typename ConnectionWrapper, typename Sink>
struct connect_source
{
    connect_source(ConnectionWrapper db, Sink sink)
        : _sink(std::move(sink))
    { (*this)(std::move(db)); }

    void operator()(ConnectionWrapper db) 
    {
        _sink(oracle::core::connect(std::move(db.username), std::move(db.password),
                                    std::move(db.service),
                                    std::move(db.hlog)));
    }
    Sink _sink;
};

template<typename HandleLog, typename Sink>
inline auto operator>>=(connect_wrapper<HandleLog> lhs, Sink&& sink)
PICAPAU_DECLTYPE_AUTO_RETURN
( connect_source<connect_wrapper<HandleLog>, Sink>(std::move(lhs), std::forward<Sink>(sink)) )

template<typename HandleLog = handle_log>    
inline auto connect(std::string username,
                    std::string password,
                    std::string service,
                    HandleLog&& hlog = HandleLog())
PICAPAU_DECLTYPE_AUTO_RETURN
(connect_wrapper<HandleLog>
{std::move(username), std::move(password), std::move(service), std::forward<HandleLog>(hlog)})

}}}}
