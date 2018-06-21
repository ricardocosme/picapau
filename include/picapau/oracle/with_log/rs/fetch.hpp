
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/oracle/with_log/result_set.hpp"
#include <range/v3/range_for.hpp>
#include <utility>

namespace picapau { namespace oracle { namespace with_log { namespace rs {

template<typename HandleLog, typename HandleError>            
struct fetch_wrapper
{    
    HandleLog hlog;
    HandleError herror;
};
            
template<typename FetchWrapper, typename Sink>
struct fetch_impl
{
    template<typename ResultSet>
    void operator()(ResultSet&& rs)
    {
        fetch.hlog(rs.log);
        if(!rs.value.valid())
            fetch.herror(rs.value.get_unexpected().value());
        else
        {
            RANGES_FOR(auto&& tuple, rs.value.value())
            {
                fetch.hlog(tuple.log);
                if(!tuple.value.valid())
                    fetch.herror(tuple.value.get_unexpected().value());
                else sink(std::move(tuple.value.value()));
            }
        }
    }
    FetchWrapper fetch;
    Sink sink;
};

template<typename HandleLog, typename HandleError>
inline auto fetch(HandleLog&& hlog, HandleError&& herror)
PICAPAU_DECLTYPE_AUTO_RETURN
( fetch_wrapper<HandleLog, HandleError>{std::forward<HandleLog>(hlog), std::forward<HandleError>(herror)} )
                
}}}}

template<typename HandleLog,
         typename HandleError,
         typename Sink>
inline auto operator>>=(picapau::oracle::with_log::rs::fetch_wrapper<
                            HandleLog, HandleError> fetchw,
                        Sink&& sink)
PICAPAU_DECLTYPE_AUTO_RETURN
( picapau::oracle::with_log::rs::fetch_impl<
      picapau::oracle::with_log::rs::fetch_wrapper<HandleLog, HandleError>, Sink>
  {std::move(fetchw), std::forward<Sink>(sink)} )
