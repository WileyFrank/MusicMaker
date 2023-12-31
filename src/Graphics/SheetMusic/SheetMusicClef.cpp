#include "SheetMusicClef.h"

#include <iostream>


SheetMusicClef::SheetMusicClef()
	:staffX(0), staffY(0), staffHeight(180), clef(TrebleClef)
{
	loadClef();

}

SheetMusicClef::SheetMusicClef(float x, float y, float width, float height, Clef clef)
	:staffX(x), staffY(y), staffHeight(height), clef(clef),
	color(sf::Color(0,0,0)), hoverColor(sf::Color(0, 0, 0))
{
	loadClef();
}

void SheetMusicClef::render()
{
	update();
	draw();
}

void SheetMusicClef::update()
{
	if (this->hover)
	{
		sprite.setColor(hoverColor);
		unhover = true;
		return;
	}
	if (this->unhover)
	{
		sprite.setColor(color);
		unhover = false;
	}
}

void SheetMusicClef::draw()
{
	window->draw(sprite);
}

std::pair<sf::Vector2f, sf::Vector2f> SheetMusicClef::getHoverArea()
{
	sf::Vector2f position(
		sprite.getPosition().x - sprite.getOrigin().x * sprite.getScale().x,
		staffY);

	sf::Vector2f size(
		sprite.getLocalBounds().width * sprite.getScale().x,
		staffHeight);



	return std::pair<sf::Vector2f, sf::Vector2f>(position, size);
}


void SheetMusicClef::loadClef()
{
	float newScale;

	//This generates points such that the clef sits properly when the position is set to the top left of the staff
	switch (clef)
	{
	case TrebleClef:

		if (!texture.loadFromFile("resources/images/sheet_music/TrebleClef.png")) {
			// Handle error
			std::cerr << "Clef Texture unable to load\n";
		}

		sprite.setTexture(texture);
		sprite.setColor(sf::Color(255, 255, 255, 255));

		newScale = this->staffHeight * 2.00f / sprite.getLocalBounds().height;

		sprite.setScale(newScale, newScale);
		
		staffPoint = 0.75f; //this indicates that the clef should sit on the 4th of 5 lines

		clefPoint = 0.65f; //this indicates that the point to side on the staffPoint is 65% of the way down the sprite

		sprite.setOrigin(sf::Vector2f(0, sprite.getLocalBounds().height * clefPoint)); //sets the origin to the "center" of the symbol, the part which rests on a note

		sprite.setPosition(this->staffX + staffHeight / 10.0f, (this->staffY + (this->staffHeight * staffPoint)));
		break;

	case BassClef:

		if (!texture.loadFromFile("resources/images/sheet_music/BassClef.png")) {
			// Handle error
			std::cerr << "Clef Texture unable to load\n";
		}

		sprite.setTexture(texture);
		sprite.setColor(sf::Color(255, 255, 255, 255));

		newScale = this->staffHeight * 0.8f / sprite.getLocalBounds().height;

		sprite.setScale(newScale, newScale);

		staffPoint = 0.25f; //this indicates that the clef should sit on the 4th of 5 lines

		clefPoint = 0.315f; //this indicates that the point to side on the staffPoint is 65% of the way down the sprite

		sprite.setOrigin(sf::Vector2f(0, sprite.getLocalBounds().height * clefPoint)); //sets the origin to the "center" of the symbol, the part which rests on a note

		sprite.setPosition(this->staffX + staffHeight / 10.0f, (this->staffY + (this->staffHeight * staffPoint)));
		break;
	}
}

void SheetMusicClef::setColor(sf::Color color)
{
	if (this->color == this->hoverColor) 
		this->hoverColor = color;
	this->color = color; 
	sprite.setColor(color);
}