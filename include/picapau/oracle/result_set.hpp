
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/oracle/detail/fetch.hpp"
#include "picapau/oracle/detail/result_set.hpp"
#include <string>

namespace picapau { namespace oracle { 

namespace detail {        
    template<typename Tuple>
    struct column_fetch
    {
        template<typename Column>
        void operator()(Column& column) const
        {
            if(!tuple) return;
        
            auto fetched = fetch<Column>(rs, idx++);
            if(fetched) column = fetched.value();
            else tuple = fetched.get_unexpected();
        }
        ::oracle::occi::ResultSet& rs;
        std::size_t& idx;
        Tuple& tuple;   
    };

    struct Identity
    {
        template<typename T>
        T operator()(T&& in) const
        { return std::forward<T>(in); }
    };

    template<typename Tuple>
    using expected_tuple = boost::expected<Tuple, std::string>;
}
        
template<typename... Columns>
using result_set = detail::result_set<
    detail::expected_tuple,
    detail::column_fetch,
    detail::Identity,
    Columns...
>;

}}
