
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/oracle/detail/result_set.hpp"
#include "picapau/oracle/with_log/detail/fetch.hpp"
#include <boost/fusion/include/io.hpp>
#include <sstream>

namespace picapau { namespace oracle { namespace with_log {

namespace detail {        
    template<typename Tuple>
    struct column_fetch
    {
        template<typename Column>
        void operator()(Column& column) const
        {
            if(!tuple.value) return;
        
            auto fetched = detail::fetch<Column>(rs, idx++);
            tuple.log += fetched.log;
            if(fetched.value) column = fetched.value.value();
            else tuple.value = fetched.value.get_unexpected();
        }
        ::oracle::occi::ResultSet& rs;
        std::size_t& idx;
        Tuple& tuple;   
    };

    struct AppendLogTuple
    {
        template<typename T>
        T operator()(T&& in) const
        {
            std::stringstream stream;
            if(in.value.valid())
                stream << in.value.value() << " tuple fetched";
            in.log += T::logln("fetch", stream.str());
            return std::forward<T>(in);
        }
    };
    template<typename Tuple>
    using with_log_expected_tuple = picapau::with_log<boost::expected<Tuple, std::string>>;
}
            
template<typename... Columns>
using result_set = picapau::oracle::detail::result_set<
    detail::with_log_expected_tuple,
    detail::column_fetch,
    detail::AppendLogTuple,
    Columns...
>;
            
}}}
