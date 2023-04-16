#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <cassert>
#include <dlfcn.h>

#include "debug.hh"

namespace xui
{

class DynamicLoader
{
public:
    static DynamicLoader* instance()
    {
        static DynamicLoader loader;

        return &loader;
    }

    void load( const std::string& path, std::string name)
    {
        $D( "Trying to load %s (%s)\n", path.data(), name.data());
        void* handle = dlopen( path.data(), RTLD_NOW | RTLD_GLOBAL);
        char* error = dlerror();
        $D( "Loading: %s\n", getMessage( error));

        handles_.insert( {std::move( name), handle});
    }

    void enqueueUnload( const std::string& name)
    {
        auto handle = handles_.find( name);
        if ( handle == handles_.end() )
            assert( 0 && "No plugin with such name");

        unload_queue_.push_back( handle->second);
    }

    void commitUnload()
    {
        for ( auto handle : unload_queue_)
            unload( handle);

        unload_queue_.clear();
    }

private:
    std::unordered_map<std::string, void*> handles_;
    std::vector<void*> unload_queue_;

    DynamicLoader() = default;

    static const char* getMessage( char* error)
    {
        return (error) ? error : "No error";
    }

    void unload( void* handle)
    {
        char* error = dlerror();
        $D( "Trying to unload: %s\n", getMessage( error));
        dlclose( handle);
        error = dlerror();
        $D( "Unloading: %s\n", getMessage( error));
    }
};

} // namespace xui
