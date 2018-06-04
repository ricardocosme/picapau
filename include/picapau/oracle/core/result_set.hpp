
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/fusion/include/as_vector.hpp>
#include <boost/mpl/vector.hpp>
#include <occi.h>
#include <string>

namespace picapau { namespace oracle { namespace core {

//RAII for ::oracle::occi::ResultSet
//This type isn't Copyable but it is Movable
template<typename... Columns>        
struct result_set
{
    using columns_t = boost::mpl::vector<Columns...>;
    using tuple_t = typename boost::fusion::result_of::as_vector<
        columns_t>::type;
    using expected_tuple_t = boost::expected<tuple_t, std::string>;
    
    result_set() = default;
    
    result_set(::oracle::occi::Statement& stmt,
               ::oracle::occi::ResultSet& rs)
        : _stmt(&stmt)
        , _rs(&rs)
    {}
    
    result_set(result_set&& rhs) noexcept
        : _stmt(rhs._stmt)
        , _rs(rhs._rs)
    {
        rhs._stmt = nullptr;
        rhs._rs = nullptr;
    }
    
    result_set& operator=(result_set&& rhs) noexcept
    {
        _stmt = rhs._stmt;
        _rs = rhs._rs;
        rhs._stmt = nullptr;
        rhs._rs = nullptr;
        return *this;
    }
    
    ~result_set()
    {
        if(_stmt == nullptr) return;

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
    
    ::oracle::occi::Statement* stmt() const noexcept
    { return _stmt; }
    
private:
    
    ::oracle::occi::Statement* _stmt{nullptr};
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
