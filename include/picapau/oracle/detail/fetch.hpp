#pragma once

#include "picapau/detail/make_expected_from_call.hpp"
#include <occi.h>
#include <cstddef>

namespace picapau { namespace oracle { namespace detail {

template<typename Column>
Column fetch_impl(::oracle::occi::ResultSet& rs, std::size_t idx);
    
template<>
int fetch_impl<int>(::oracle::occi::ResultSet& rs, std::size_t idx)
{ return rs.getInt(idx); }
    
template<>
float fetch_impl<float>(::oracle::occi::ResultSet& rs, std::size_t idx)
{ return rs.getFloat(idx); }
    
template<>
std::string fetch_impl<std::string>(::oracle::occi::ResultSet& rs, std::size_t idx)
{ return rs.getString(idx); }
    
//TODO:std::error_code instead of std::string
template<typename Column>        
inline boost::expected<Column, std::string>
fetch(::oracle::occi::ResultSet& rs, std::size_t idx)
{
    return picapau::detail::make_expected_from_call
        ([&rs, idx]{return fetch_impl<Column>(rs, idx);});
}
    
}}}
