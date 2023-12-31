#include "ResourceManager.h"

std::map<std::string, sf::Texture> ResourceManager::textures;
std::map<std::string, sf::Font> ResourceManager::fonts;

sf::Texture* ResourceManager::getTexture(std::string filepath)
{
    // Check if the font is already loaded
    auto it = textures.find(filepath);
    if (it != textures.end())
    {
        // Font is already loaded, return the existing font
        return &(it->second);
    }

    // Font is not loaded, load it now
    sf::Texture texture;
    if (!texture.loadFromFile(filepath))
    {
        // Handle error
        std::cout << filepath << " font unable to load\n";
        return nullptr;
    }

    // Insert the loaded font into the map and return a pointer to it
    auto inserted = textures.insert(std::make_pair(filepath, std::move(texture)));
    return &(inserted.first->second);
}

sf::Font* ResourceManager::getFont(std::string filepath)
{
    // Check if the font is already loaded
    auto it = fonts.find(filepath);
    if (it != fonts.end())
    {
        // Font is already loaded, return the existing font
        return &(it->second);
    }

    // Font is not loaded, load it now
    sf::Font font;
    if (!font.loadFromFile(filepath))
    {
        // Handle error
        std::cout << filepath << " font unable to load\n";
        return nullptr;
    }

    // Insert the loaded font into the map and return a pointer to it
    auto inserted = fonts.insert(std::make_pair(filepath, std::move(font)));
    return &(inserted.first->second);
}
