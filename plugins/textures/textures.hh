#pragma once

#include "plugins/tools/tools.hh"
#include "core/lru_cache.hh"

namespace xui
{

class TexturesCache final
{
public:
    TexturesCache()
        : cache_{ 100}
    {}

    const sf::Texture*
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
    const sf::Texture*
    load( const std::string& path)
    {
        auto texture = std::make_unique<sf::Texture>();
        if ( !texture->loadFromFile( path) )
        {
            std::fprintf( stderr, "Failed to load texture: %s\n", path.c_str());
            assert( 0 && "will be removed");
        }

        cache_.insert( path, std::move( texture));
        return cache_.front().second.get();
    }

private:
    LRUCache<std::string, std::unique_ptr<sf::Texture>> cache_;
};

class TexturesPlugin final
    : public IPlugin
{
public:
    static const int ID;

    TexturesPlugin() = default;
    ~TexturesPlugin() = default;

    void deserialize( const json&) override {}
    void serialize( json&) override {}

    const sf::Texture*
    getTexture( const std::string& path)
    {
        return cache_.find( path);
    }

    void dump() { cache_.dump(); }

private:
    TexturesCache cache_;
};

}
