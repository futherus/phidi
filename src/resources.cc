#include <assert.h>

#include "resources.h"

namespace gui
{

std::map<std::string, std::unique_ptr<sf::Texture>> TextureFactory::textures_;

bool TextureFactory::loadTexture( const std::string key,
                                  const std::string path,
                                  const sf::IntRect& bounds)
{
    auto tmp = std::make_unique<sf::Texture>();

    if ( !tmp->loadFromFile( path, bounds))
    {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return false;
    }

    textures_.insert( std::make_pair( key, std::move(tmp)));

    return true;
}

const sf::Texture* TextureFactory::getTexture( const std::string& key)
{
    return textures_.at(key).get();
}

std::map<std::string, std::unique_ptr<sf::Font>> FontFactory::fonts_;

bool FontFactory::loadFont( const std::string key,
                            const std::string path)
{
    auto tmp = std::make_unique<sf::Font>();

    if ( !tmp->loadFromFile( path))
    {
        std::cerr << "Failed to load font: " << path << std::endl;
        return false;
    }

    fonts_.insert( std::make_pair( key, std::move(tmp)));

    return true;
}

sf::Font* FontFactory::getFont( const std::string& key)
{
    return fonts_.at( key).get();
}

} // namespace gui
