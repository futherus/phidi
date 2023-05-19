#pragma once
#include <string>
#include "nlohmann/json.hpp"

using Json = nlohmann::ordered_json;

namespace xui {

std::string
Demangle( const char* mangled);

/**
 * Dump object's type.
 */
template <typename T>
Json
DumpInfo( const T&)
{
    Json info{};
    info["type"] = Demangle( typeid( T).name());
    return info;
}

} // namespace xui


