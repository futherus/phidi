#ifndef RESOURCES_H
#define RESOURCES_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <memory>

namespace gui
{

class TextureFactory
{
private:
    static std::map<std::string, std::unique_ptr<sf::Texture>> textures_;

    TextureFactory() = default;

public:
    static bool loadTexture(const std::string key,
                            const std::string path,
                            const sf::IntRect& bounds);

    static const sf::Texture* getTexture(const std::string& key);
};

class FontFactory
{
private:
    static std::map<std::string, std::unique_ptr<sf::Font>> fonts_;

    FontFactory() = default;

public:
    static bool loadFont(const std::string key,
                         const std::string path);

    static sf::Font* getFont(const std::string& key);
};

} // namespace gui

#endif // RESOURCES_H