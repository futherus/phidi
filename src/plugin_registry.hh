#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <cassert>
#include <functional>

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
public:
    static PluginRegistry* instance()
    {
        static PluginRegistry registry;

        return &registry;
    }

    /**
     * FIXME: RegisterPlugin should have exlusive right
     *        to access addPlugin().
     */
    template <typename T>
    void addPlugin( std::unique_ptr<T> plugin,
                    std::string name)
    {
        static_assert( std::is_base_of<IPlugin, T>::value
                       && !std::is_same<IPlugin, T>::value,
                       "T should be derived from IPlugin.");

        assert( plugins_.find( &T::ID) == plugins_.end()
                && "Plugin is already loaded.");
        assert( plugin_names_.find( name) == plugin_names_.end()
                && "Plugin with the same name is already loaded.");

        plugin_names_.insert( {std::move( name), plugin.get()});
        plugins_.insert( {&T::ID, std::move( plugin)});
    }

    template<typename T>
    T* getPlugin()
    {
        static_assert( std::is_base_of<IPlugin, T>::value
                       && !std::is_same<IPlugin, T>::value);

        auto tmp = plugins_.find( &T::ID);
        assert( tmp != plugins_.end() && "Plugin was not found.");

        return static_cast<T*>( tmp->second.get());
    }

    IPlugin* getPlugin( const std::string& name)
    {
        auto tmp = plugin_names_.find( name);
        assert( tmp != plugin_names_.end() && "Plugin was not found.");

        return tmp->second;
    }

private:
    std::unordered_map<const void*, std::unique_ptr<IPlugin>> plugins_;
    std::unordered_map<std::string, IPlugin*> plugin_names_;

    PluginRegistry() = default;
};

template <typename T>
class RegisterPlugin
{
public:
    RegisterPlugin( std::string name)
    {
        /**
         * There is no way to do it with static_assert.
         * See: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1045r1.html.
         */
        assert( name != "" && "Plugin name cannot be empty");
        PluginRegistry::instance()->addPlugin<T>( std::make_unique<T>(), std::move( name));
    }
};

} // namespace xui
