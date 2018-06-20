#pragma once

#include "picapau/detail/to_string.hpp"
#include "picapau/oracle/detail/fetch.hpp"
#include "picapau/support/with_log.hpp"

namespace picapau { namespace oracle { namespace with_log { namespace detail {
    
template<typename Column,
         template <typename> class WithLog = picapau::with_log>
inline WithLog<boost::expected<Column, std::string>>
fetch(::oracle::occi::ResultSet& rs, std::size_t idx)
{
    using with_log_t = WithLog<boost::expected<Column, std::string>>;
    
    auto res = oracle::detail::fetch<Column>(rs, idx);
    
    std::string log;
    if(res) log += with_log_t::logln("fetch", "'" + picapau::detail::to_string(res.value()) +
                         "' fetched from " +
                         " column=" + picapau::detail::to_string(idx));
    else
        log += with_log_t::logln("fetch", "Fetch failed for column=" + picapau::detail::to_string(idx)) +
            with_log_t::logln("fetch", "Error: " + res.get_unexpected().value());
    
    return {std::move(res), std::move(log)};
}
    
}}}}
