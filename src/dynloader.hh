#pragma once

#include <vector>
#include <string>
#include <dlfcn.h>

#include "debug.hh"

namespace xui
{

class DynamicLoader
{
private:
    std::vector<void*> handles_;

    DynamicLoader() = default;

    static const char* getMessage( char* error)
    {
        return (error) ? error : "No error";
    }

public:
    static DynamicLoader* instance()
    {
        static DynamicLoader loader;

        return &loader;
    }

    /**
     * FIXME: Unloading should be performed after all
     *        objects were destroyed.
     *        Futhermore, usually unloading is no-op.
     */
    ~DynamicLoader()
    {
        // for ( size_t i = 1; i < handles_.size(); i++ )
        //     unload( handles_.at(i));
        for ( auto handle : handles_ )
            unload( handle);
    }

    void unload( void* handle)
    {
        char* error = dlerror();
        $D( "Trying to unload: %s\n", getMessage( error));
        dlclose( handle);
        error = dlerror();
        $D( "Unloading: %s\n", getMessage( error));
    }

    void load( std::string path)
    {
        $D( "Trying to load %s\n", path.data());
        void* handle = dlopen( path.data(), RTLD_NOW | RTLD_GLOBAL);
        char* error = dlerror();
        $D( "Loading: %s\n", getMessage( error));

        handles_.push_back( handle);
    }
};

} // namespace xui
