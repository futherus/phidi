#ifndef LOADER_HH
#define LOADER_HH

#include <vector>
#include <string>
#include <dlfcn.h>

#include "debug.hh"

namespace xui
{

class Loader
{
private:
    std::vector<void*> handles_;

    Loader() = default;

    static const char* getMessage( char* error)
    {
        return (error) ? error : "No error";
    }

public:
    static Loader* getLoader()
    {
        static Loader loader;

        return &loader;
    }

    // FIXME: unloading should be performed after all
    //        objects were destroyed.
    //        Futhermore, usually unloading is no-op.
    ~Loader() = default;

    // void unload( void* handle)
    // {
    //     char* error = dlerror();
    //     $D( "Trying to unload: %s\n", getMessage( error));
    //     dlclose( handle);
    //     error = dlerror();
    //     $D( "Unloading: %s\n", getMessage( error));
    // }

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

#endif // LOADER_HH
