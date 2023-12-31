#pragma once

#include <vector>
#include "SFML/Graphics.hpp"
#include "SheetMusicElement.h"
#include "../../Music/MusicUtilities.h"


//The clef of a staff is defined by elements of the staff, not the clef
//In particular, the position of the center of the clef is determined based on the position of the staff and bar
class SheetMusicClef : public SheetMusicElement
{
private:
	float staffX, staffY, staffHeight, staffPoint, clefPoint;
	Clef clef;


	sf::Sprite sprite;
	sf::Texture texture;

	sf::Color color, hoverColor;
	
	void loadClef();

public:
	SheetMusicClef();
	SheetMusicClef(float x, float y, float width, float height, Clef clef);

	void render() override;
	void update() override;
	void draw() override;
	
	
	std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() override;


	Clef getClef() { return clef; }
	float getWidth() { return sprite.getLocalBounds().width * sprite.getScale().x; }
	
	void setColor(sf::Color color);
	void setHoverColor(sf::Color color) { this->hoverColor = color; }

};

