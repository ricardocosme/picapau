
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/detail/macro_decltype_auto_return.hpp"
#include "picapau/oracle/core/connect.hpp"
#include <string>

namespace picapau { namespace oracle { namespace rs {

struct connect_wrapper
{ std::string username, password, service; };
                
template<typename Sink>
struct connect_source
{
    connect_source(connect_wrapper db, Sink sink)
        : _sink(std::move(sink))
    { (*this)(std::move(db)); }

    void operator()(connect_wrapper db) 
    {
        _sink(oracle::core::connect
              (std::move(db.username), std::move(db.password),
               std::move(db.service)));
    }
    Sink _sink;
};

template<typename Sink>
inline auto operator>>=(connect_wrapper lhs, Sink&& sink)
PICAPAU_DECLTYPE_AUTO_RETURN
( connect_source<Sink>(std::move(lhs), std::forward<Sink>(sink)) )
    
inline auto connect(std::string username,
                    std::string password,
                    std::string service)
PICAPAU_DECLTYPE_AUTO_RETURN
(connect_wrapper
 {std::move(username), std::move(password), std::move(service)})

}}}
