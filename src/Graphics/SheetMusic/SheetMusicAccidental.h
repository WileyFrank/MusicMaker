#pragma once

#include "SFML/Graphics.hpp"
#include "SheetMusicElement.h"
#include "../../Music/MusicUtilities.h"


class SheetMusicAccidental : public SheetMusicElement
{
private:
	float staffY, staffHeight; //the topPosition and height of the staff

	//the position of the symbol in terms of the staff where 0.0 is top and 1.0 is bottom. 
	// 1.25 = Middle C in treble clef
	float positionX, positionY; 


	//used to change the width and gap from note
	float widthFactor = 1.0f;

	Accidental accidental;

	sf::Color color;
	sf::Color hoverColor;
	sf::Sprite sprite;
	sf::Texture texture;


	void loadAccidental();
public:
	SheetMusicAccidental(Accidental accidental = Natural)
		:accidental(accidental), positionX(0), positionY(0), staffHeight(0), staffY(0){};
	SheetMusicAccidental(float staffY, float staffHeight, float positionX, float positionY, Accidental accidental, sf::Color color = sf::Color(255,255,255));

	void draw() override;
	std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() override;

	float getWidth() { return sprite.getLocalBounds().width * sprite.getScale().x * widthFactor; }

	void setPosition(float x, float y);
	void setColor(sf::Color color) { this->color = color; sprite.setColor(color); }
	void setHoverColor(sf::Color color) { this->hoverColor = hoverColor; }
};

