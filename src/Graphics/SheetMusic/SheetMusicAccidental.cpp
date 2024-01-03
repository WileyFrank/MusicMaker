#include "SheetMusicAccidental.h"
//private
void SheetMusicAccidental::loadAccidental()
{
	float notePointVertical = 1.0f; //determines the location on the note to be aligned horizontally with the note
	float scale = this->staffHeight * 0.5f;


	//loading the proper texture and position for the origin
	switch (accidental)
	{
	case DoubleFlat:
		if (!texture.loadFromFile("resources/images/sheet_music/accidentals/double_flat.png")) {
			// Handle error
			std::cout << "Accidental Texture unable to load\n";
		}
		notePointVertical = 185.0f / 256.0f;
		break;
	case Flat:
		if (!texture.loadFromFile("resources/images/sheet_music/accidentals/flat.png")) {
			// Handle error
			std::cout << "Accidental Texture unable to load\n";
		}
		notePointVertical = 185.0f / 256.0f;
		scale = this->staffHeight * 0.45f;

		break;
	case Natural:
	case ForcedNatural:
		if (!texture.loadFromFile("resources/images/sheet_music/accidentals/natural.png")) {
			// Handle error
			std::cout << "Accidental Texture unable to load\n";
		}
		notePointVertical = 0.5f;

		break;
	case Sharp:
		if (!texture.loadFromFile("resources/images/sheet_music/accidentals/sharp.png")) {
			// Handle error
			std::cout << "Accidental Texture unable to load\n";
		}
		notePointVertical = 0.525f;

		break;
	case DoubleSharp:
		if (!texture.loadFromFile("resources/images/sheet_music/accidentals/double_sharp.png")) {
			// Handle error
			std::cout << "Accidental Texture unable to load\n";
		}
		scale = this->staffHeight * 0.25f;
		notePointVertical = 0.5f;
		break;
	}

	this->sprite.setTexture(texture);
	float newScale = scale / sprite.getLocalBounds().height;
	this->sprite.setScale(newScale, newScale);
	this->sprite.setOrigin(sf::Vector2f(0.0f, sprite.getLocalBounds().height * notePointVertical)); //sets the origin to the "center" of the symbol, the part which rests on a note
	this->sprite.setColor(color);

	this->sprite.setPosition(sf::Vector2f(this->x + sprite.getOrigin().x * sprite.getScale().x, staffY + (this->y)*staffHeight));
}

//constructors
SheetMusicAccidental::SheetMusicAccidental(float staffY, float staffHeight, float positionX, float positionY, Accidental accidental, sf::Color color)
	:staffY(staffY), staffHeight(staffHeight), accidental(accidental), color(color)
{
	this->x = positionX;
	this->y = positionY;
	this->height = staffHeight;
	
	loadAccidental();
}

void SheetMusicAccidental::draw()
{
	window->draw(sprite);
}

std::pair<sf::Vector2f, sf::Vector2f> SheetMusicAccidental::getHoverArea()
{
	sf::Vector2f position(
		sprite.getPosition().x - sprite.getOrigin().x * sprite.getScale().x,
		sprite.getPosition().y - sprite.getOrigin().y * sprite.getScale().y);

	sf::Vector2f size(
		sprite.getLocalBounds().width * sprite.getScale().x,
		sprite.getLocalBounds().height * sprite.getScale().y);

	return std::pair<sf::Vector2f, sf::Vector2f>(position, size);
}

void SheetMusicAccidental::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;
	sprite.setPosition(sf::Vector2f(x, y * staffHeight + staffY));
}
