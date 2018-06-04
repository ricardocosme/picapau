
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "picapau/oracle/core/session.hpp"
#include <ostream>

namespace picapau { namespace oracle { namespace core {
        
inline std::ostream&
operator<<(std::ostream& os, const session& s)
{ return os << "{env=" << s.env() << " conn=" << s.conn() << "}" << std::endl; }
        
}}}
