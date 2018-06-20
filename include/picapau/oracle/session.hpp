
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <occi.h>

namespace picapau { namespace oracle {
            
//RAII for ::oracle::occi::Environment and ::oracle::occi::Connection
//This type isn't Copyable but it is Movable
struct session {
    
    session() = default;
    
    session(::oracle::occi::Environment& env,
            ::oracle::occi::Connection& conn)
        : _env(&env)
        , _conn(&conn)
    {}
    
    session(session&& rhs) noexcept
        : _env(rhs._env)
        , _conn(rhs._conn)
    {
        rhs._env = nullptr;
        rhs._conn = nullptr;
    }
    
    session& operator=(session&& rhs) noexcept
    {
        _env = rhs._env;
        _conn = rhs._conn;
        rhs._env = nullptr;
        rhs._conn = nullptr;
        return *this;
    }
    
    ~session()
    {
        if(_env == nullptr) return;

        _env->terminateConnection(_conn);
        ::oracle::occi::Environment::terminateEnvironment(_env); 
    }
    
    ::oracle::occi::Environment* env() const noexcept
    { return _env; }
    
    ::oracle::occi::Connection* conn() const noexcept
    { return _conn; }
    
private:
    
    ::oracle::occi::Environment* _env{nullptr};
    ::oracle::occi::Connection* _conn{nullptr};
};

inline bool operator==(const session& lhs, const session& rhs)
{ return lhs.env() == rhs.env() && lhs.conn() == rhs.conn(); }

inline bool operator!=(const session& lhs, const session& rhs)
{ return !(lhs == rhs); }

}}
