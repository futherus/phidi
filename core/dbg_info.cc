#include <regex>
#include <cxxabi.h>
#include "fmt/core.h"
#include "nlohmann/json.hpp"

#include "../core/dbg_info.hh"

namespace xui {

std::string
Demangle( const char* mangled)
{
    std::string demangled = mangled;
    char* name = abi::__cxa_demangle( mangled, nullptr, nullptr, nullptr);
    if ( name )
    {
        try
        {
            demangled = name;
        } catch ( ... )
        {
            std::free( name);
            throw;
        }
    }

    std::free( name);
    return demangled;
}

} // namespace xui
