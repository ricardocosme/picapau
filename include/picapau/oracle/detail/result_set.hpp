
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/detail/macro_decltype_auto_return.hpp"
#include "picapau/oracle/detail/fetch.hpp"
#include "picapau/oracle/statement.hpp"
#include "picapau/support/with_log.hpp"
#include <boost/fusion/include/mpl.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <occi.h>
#include <range/v3/view_facade.hpp>
#include <string>
#include <range/v3/all.hpp>

namespace picapau { namespace oracle { namespace detail {

struct cursor_base {};

template<typename T>
typename T::value_type&
get_tuple(T& etuple,
          typename std::enable_if<boost::is_expected<T>::value>::type* = 0)
{ return etuple.value(); }
            
template<typename T>
typename T::value_type::value_type&
get_tuple(T& etuple,
          typename std::enable_if<picapau::is_with_log<T>::value>::type* = 0)
{ return etuple.value.value(); }
            
template<template <typename> class Monad,
         template <typename> class ColumnFetch,
         typename Transform,
         typename... Columns>        
struct result_set : ranges::view_facade<result_set<Monad, ColumnFetch, Transform, Columns...>>
{    
    using columns_t = boost::mpl::vector<Columns...>;
    using tuple_t = typename boost::fusion::result_of::as_vector<
        columns_t>::type;
    using expected_tuple_t = Monad<tuple_t>;

private:

    friend ranges::range_access;
    
    struct cursor : cursor_base
    {
        ::oracle::occi::ResultSet* _rs{nullptr};

        cursor() = default;
        
        cursor(::oracle::occi::ResultSet* rs)
            : _rs(rs)
        { next(); }
        
        expected_tuple_t read() const
        {
            std::size_t idx{1};
            expected_tuple_t exptuple(tuple_t{});
            boost::fusion::for_each(get_tuple(exptuple),
                                    ColumnFetch<expected_tuple_t>{*_rs, idx, exptuple});            
            return Transform{}(std::move(exptuple));
        }
        
        bool equal(ranges::default_sentinel) const
        { return _rs ? (_rs->status() == ::oracle::occi::ResultSet::END_OF_FETCH) : false ; }

        void next()
        { _rs->next(); }
    };

    cursor begin_cursor() const
    { return {_rs}; }
    
public:
    
    result_set() = default;
    
    result_set(statement stmt,
               ::oracle::occi::ResultSet& rs)
        : _stmt(std::move(stmt))
        , _rs(&rs)
    {}
    
    result_set(result_set&& rhs) noexcept
        : _stmt(std::move(rhs._stmt))
        , _rs(rhs._rs)
    {
        rhs._rs = nullptr;
    }
    
    result_set& operator=(result_set&& rhs) noexcept
    {
        _stmt = std::move(rhs._stmt);
        _rs = rhs._rs;
        rhs._rs = nullptr;
        return *this;
    }
    
    ~result_set()
    {
        if(_stmt == statement()) return;

        _stmt->closeResultSet(_rs);
    }
        
    ::oracle::occi::ResultSet* get() const noexcept
    { return _rs; }

    ::oracle::occi::ResultSet& operator*() const noexcept
    {
        assert(_rs != nullptr);
        return *_rs;
    }
    
    ::oracle::occi::ResultSet* operator->() const noexcept
    {
        assert(_rs != nullptr);
        return _rs;
    }
    
    const statement& stmt() const noexcept
    { return _stmt; }

private:
    
    statement _stmt;
    ::oracle::occi::ResultSet* _rs{nullptr};
};

template<typename... Columns>            
inline bool operator==(const result_set<Columns...>& lhs,
                       const result_set<Columns...>& rhs)
{ return lhs.stmt() == rhs.stmt() && lhs.get() == rhs.get(); }

template<typename... Columns>                        
inline bool operator!=(const result_set<Columns...>& lhs,
                       const result_set<Columns...>& rhs)
{ return !(lhs == rhs); }

}}}
