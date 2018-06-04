#pragma once

#include "picapau/detail/make_expected_from_call.hpp"
#include <occi.h>
#include <cstddef>

namespace picapau { namespace oracle { namespace core {

namespace detail {

template<typename Column>
Column fetch(::oracle::occi::ResultSet& rs, std::size_t idx);
    
template<>
int fetch<int>(::oracle::occi::ResultSet& rs, std::size_t idx)
{ return rs.getInt(idx); }
    
template<>
std::string fetch<std::string>(::oracle::occi::ResultSet& rs, std::size_t idx)
{ return rs.getString(idx); }
    
} //namespace detail    
        
//TODO:std::error_code instead of std::string
template<typename Column>        
inline boost::expected<Column, std::string>
fetch(::oracle::occi::ResultSet& rs, std::size_t idx)
{
    return picapau::detail::make_expected_from_call
        ([&rs, idx]{return detail::fetch<Column>(rs, idx);});
}
    
}}}
