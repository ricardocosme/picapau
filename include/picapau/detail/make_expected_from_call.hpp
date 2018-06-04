
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/expected/expected.hpp>
#include <exception>
#include <string>
#include <type_traits>

namespace picapau { namespace detail {

//TODO:std::error_code instead of std::string
template<typename F>
inline typename std::enable_if<
    !std::is_same<typename std::result_of<F()>::type, void>::value,
    boost::expected<typename std::result_of<F()>::type, std::string>
    >::type
make_expected_from_call(F&& f) noexcept
{
    using T = typename std::result_of<F()>::type;
    using E = boost::expected<typename std::result_of<F()>::type, std::string>;
    try
    { return E(f()); }
    catch(const std::exception& e)
    { return boost::make_expected_from_error<T>(std::string(e.what())); }
    catch(...)
    {
        return boost::make_expected_from_error<T>
            (std::string("unknown exception"));
    }
}
            
//TODO:std::error_code instead of std::string
template<typename F>
inline typename std::enable_if<
    std::is_same<typename std::result_of<F()>::type, void>::value,
    boost::expected<void, std::string>
    >::type
make_expected_from_call(F&& f) noexcept
{
    using T = typename std::result_of<F()>::type;
    using E = boost::expected<typename std::result_of<F()>::type, std::string>;
    try
    {
        f();
        return E();
    }
    catch(const std::exception& e)
    { return boost::make_expected_from_error<T>(std::string(e.what())); }
    catch(...)
    {
        return boost::make_expected_from_error<T>
            (std::string("unknown exception"));
    }
}
        
}}
