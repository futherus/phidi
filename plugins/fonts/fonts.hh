#pragma once

#include <memory>
#include <utility>
#include <cassert>
#include <SFML/Graphics/Font.hpp>
#include "core/lru_cache.hh"
#include "core/plugin_registry.hh"

namespace xui
{

class FontsCache final
{
public:
    FontsCache()
        : cache_{ 100}
    {}

    const sf::Font*
    find( const std::string& path)
    {
        auto it = cache_.find( path);
        if ( it != cache_.end() )
        {
            return it->second.get();
        }

        return load( path);
    }

    void dump() const
    {
        for ( auto& entry : cache_ )
        {
            std::fprintf( stderr, "path: %s addr: %p\n",
                          entry.first.c_str(), entry.second.get());
        }
    }

private:
    const sf::Font*
    load( const std::string& path)
    {
        auto font = std::make_unique<sf::Font>();
        if ( !font->loadFromFile( path) )
        {
            std::fprintf( stderr, "Failed to load font: %s\n", path.c_str());
            assert( 0 && "will be removed");
        }

        cache_.insert( path, std::move( font));
        return cache_.front().second.get();
    }

private:
    LRUCache<std::string, std::unique_ptr<sf::Font>> cache_;
};

class FontsPlugin final
    : public IPlugin
{
public:
    static const int ID;

    FontsPlugin() = default;
    ~FontsPlugin() = default;

    void deserialize( const json&) override {}
    void serialize( json&) override {}

    const sf::Font*
    getFont( const std::string& path)
    {
        return cache_.find( path);
    }

    void dump() { cache_.dump(); }

private:
    FontsCache cache_;
};

}
