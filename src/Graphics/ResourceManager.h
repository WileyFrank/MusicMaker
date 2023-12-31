#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>

class ResourceManager
{
	//static texture and fonts
	static std::map<std::string, sf::Texture> textures;
	static std::map<std::string, sf::Font> fonts;

public:
	static sf::Texture* getTexture(std::string filepath);
	static sf::Font* getFont(std::string filepath);

};

