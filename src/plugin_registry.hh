#ifndef PLUGIN_REGISTRY_HH
#define PLUGIN_REGISTRY_HH

#include <unordered_map>
#include <string>
#include <memory>
#include <cassert>

#include "debug.hh"
#include "../include/json/single_include/nlohmann/json.hpp"

namespace xui
{

using json = nlohmann::ordered_json;

class IPlugin
{
public:
    virtual void deserialize( const json& state) = 0;
    virtual void serialize( json& state) = 0;

    virtual ~IPlugin() = default;
};

class PluginRegistry
{
private:
    std::unordered_map<const void*, std::unique_ptr<IPlugin>> plugins_;
    std::unordered_map<std::string, IPlugin*> plugin_names_;

    PluginRegistry() = default;

public:
    static PluginRegistry* getPluginRegistry()
    {
        static PluginRegistry registry;

        return &registry;
    }

    template <typename T>
    void addPlugin( std::unique_ptr<T> plugin, std::string name)
    {
        assert( plugins_.find( &T::ID) == plugins_.end() && "plugin is already loaded");

        plugin_names_.insert( {name, plugin.get()});
        plugins_.insert( { &T::ID, std::move( plugin)});
    }

    template<typename T>
    T* getPlugin()
    {
        static_assert( std::is_base_of<IPlugin, T>::value
                       && !std::is_same<IPlugin, T>::value);

        auto tmp = plugins_.find( &T::ID);
        assert( tmp != plugins_.end() && "plugin was not found");

        return static_cast<T*>( tmp->second.get());
    }

    IPlugin* getPlugin( std::string name)
    {
        auto tmp = plugin_names_.find( name);
        assert( tmp != plugin_names_.end() && "plugin was not found");

        return tmp->second;
    }
};

template <typename T>
class RegisterPlugin
{
public:
    RegisterPlugin( std::string name)
    {
        PluginRegistry::getPluginRegistry()->addPlugin<T>( std::make_unique<T>(), name);
    }
};

} // namespace xui

#endif // PLUGIN_REGISTRY_HH
