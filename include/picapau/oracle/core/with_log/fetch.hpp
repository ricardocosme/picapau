#pragma once

#include "picapau/oracle/core/fetch.hpp"
#include "picapau/support/log.hpp"
#include <cstddef>
#include <string>
#include <type_traits>

namespace picapau { namespace oracle { namespace core { namespace with_log {

template<typename Column, typename F = handle_log> 
struct fetch
{
    //TODO:std::error_code instead of std::string
    inline typename std::result_of<
        F(boost::expected<Column, std::string>, std::string)>::type
    operator()(::oracle::occi::ResultSet& rs, std::size_t idx)
    {
        auto res = oracle::core::fetch<Column>(rs, idx);

        std::string log;
        if(res) log += logln("fetch", "'" + std::to_string(res.value()) +
                             "' fetched from " +
                             " column=" + std::to_string(idx));
        else
            log += logln("fetch", "Fetch failed for column=" + std::to_string(idx)) +
                logln("fetch", "Error: " + res.get_unexpected().value());
    
        return f(std::move(res), std::move(log));
    }
    F f;
};
        
}}}}
