
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <occi.h>

namespace picapau { namespace oracle { namespace core {

//RAII for ::oracle::occi::Statement            
//This type isn't Copyable but it is Movable
struct statement {
    
    statement() = default;
    
    statement(::oracle::occi::Connection& conn,
                     ::oracle::occi::Statement& stmt)
                      
        : _conn(&conn)
        , _stmt(&stmt)
    {}
    
    statement(statement&& rhs) noexcept
        : _conn(rhs._conn)
        , _stmt(rhs._stmt)
    {
        rhs._conn = nullptr;
        rhs._stmt = nullptr;
    }
    
    statement& operator=(statement&& rhs) noexcept
    {
        _conn = rhs._conn;
        _stmt = rhs._stmt;
        rhs._conn = nullptr;
        rhs._stmt = nullptr;
        return *this;
    }
    
    ~statement()
    {
        if(_conn == nullptr) return;

        _conn->terminateStatement(_stmt);
    }
        
    ::oracle::occi::Statement* get() const noexcept
    { return _stmt; }
    
    ::oracle::occi::Statement& operator*() const noexcept
    {
        assert(_stmt != nullptr);
        return *_stmt;
    }
    
    ::oracle::occi::Statement* operator->() const noexcept
    {
        assert(_stmt != nullptr);
        return _stmt;
    }
    
    ::oracle::occi::Connection* conn() const noexcept
    { return _conn; }
    
private:
    
    ::oracle::occi::Connection* _conn{nullptr};
    ::oracle::occi::Statement* _stmt{nullptr};
};

inline bool operator==(const statement& lhs,
                       const statement& rhs)
{ return lhs.conn() == rhs.conn() && lhs.get() == rhs.get(); }

inline bool operator!=(const statement& lhs,
                       const statement& rhs)
{ return !(lhs == rhs); }

}}}
