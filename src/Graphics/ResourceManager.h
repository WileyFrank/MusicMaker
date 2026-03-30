#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>

class ResourceManager
{
	//static texture and fonts
	static std::map<std::string, sf::Texture> textures;
	static std::map<std::string, sf::Font> fonts;

public:
	/** @param smooth If true (default), linear filtering — better for scaled UI/line art. Use false for crisp pixel-art sprites. */
	static sf::Texture* getTexture(std::string filepath, bool smooth = true);
	static sf::Font* getFont(std::string filepath);

};

