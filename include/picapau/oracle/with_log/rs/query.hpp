
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/detail/macro_decltype_auto_return.hpp"
#include "picapau/oracle/with_log/query.hpp"
#include "picapau/oracle/with_log/session.hpp"
#include "picapau/rs/transform.hpp"
#include "picapau/support/lift_with_log.hpp"

namespace picapau { namespace oracle { namespace with_log { namespace rs {

namespace detail {

template<typename... Columns>
struct lift_query
{
    std::string squery;
    
    template<typename Session>
    auto operator()(Session &s) 
    PICAPAU_DECLTYPE_AUTO_RETURN
        ( picapau::oracle::with_log::exec_query<Columns...>(s, std::move(squery)) )
};

template<typename... Columns>
struct exec_query
{
    std::string squery;
    
    template<typename ExpSession>
    auto operator()(ExpSession& es) 
    PICAPAU_DECLTYPE_AUTO_RETURN
    ( picapau::mbind(es, picapau::lift_with_log(lift_query<Columns...>{squery})) )        
};
    
} //namespace detail

template<typename... Columns>
auto query(std::string squery)
PICAPAU_DECLTYPE_AUTO_RETURN
( picapau::rs::transform(detail::exec_query<Columns...>{std::move(squery)}) )
            
}}}}
