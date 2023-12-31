#pragma once

#include "SheetMusicElement.h"
#include "../../Music/MusicUtilities.h"
#include <map>



class SheetMusicTimeSignature : public SheetMusicElement
{
private:
	float positionX, staffY, height;
	TimeSignature timeSignature;

	float width;
	
	sf::Color color, hoverColor;

	sf::Sprite numerator;
	sf::Sprite denominator;

	sf::Texture& numeratorTexture;
	sf::Texture& denominatorTexture;

public:
	SheetMusicTimeSignature(TimeSignature timeSignature = TimeSignature());
	SheetMusicTimeSignature(float x, float y, float height, TimeSignature timeSignature = TimeSignature());


	void hoverUpdate();
	void unhoverUpdate();

	
	void draw() override;
	void update() override;
	std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() override;
	RenderObject& getHoverObject() override;


	//getters
	float getWidth() { return width; }

	void setColor(sf::Color color) { this->color = color; numerator.setColor(this->color); denominator.setColor(this->color);}
	void setHoverColor(sf::Color color) { this->hoverColor = color; }




};

