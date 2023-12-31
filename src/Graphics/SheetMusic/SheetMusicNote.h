#pragma once

#include "SFML/Graphics.hpp"
#include "SheetMusicClef.h"
#include "SheetMusicAccidental.h"


#include "../../Music/MusicUtilities.h"


class SheetMusicNote : public SheetMusicElement
{
private:
	float staffX, staffY, staffHeight, notePointVertical, notePointHorizontal, halfStep, noteX;

	float positionY = -1.0f;
	Clef clef;
	Note note;

	float C4Position; //the position of c4 on the staff

	//State tracking
	int noteCount;

	Accidental accidental;
	SheetMusicAccidental SMAccidental;
	float accidentalWidth = 0.0f;

	KeySignature key;

	sf::Sprite sprite;
	sf::Texture* texture;
	sf::Color staffColor =	sf::Color(255,0,0);
	sf::Color noteColor	 =	sf::Color(255, 0, 0);
	sf::Color hoverColor =	sf::Color(255, 0, 0);
	sf::Color placeholderColor =	sf::Color(0, 0, 0);

	std::vector<sf::RectangleShape> extraLines;

	void loadNote();
public:
	SheetMusicNote();
	SheetMusicNote(float x, float y, float height, Clef clef, Note note, Accidental accidental = Natural, KeySignature key = {});

	void addLinesToNote(Pitch pitch);

	void draw() override;

	RenderObject& getHoverObject() override;
	std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() override;
	std::pair<sf::Vector2f, sf::Vector2f> getNoteBounds();


	
	void setWindow(sf::RenderWindow* window) override;

	void render() override;
	void update() override;

	void hoverUpdate();
	void unhoverUpdate();

	void hoverAction();
	void unhoverAction();


	//getters and setters
	void setStaffColor(sf::Color color);
	void setNoteColor(sf::Color color);
	void setHoverColor(sf::Color color);
	void setAccidental(Accidental accidental);

	void setNoteCount(int count) { this->noteCount = count; }
	void setX(float x) { noteX = x; }

	float getAccidentalWidth() { return accidentalWidth; }
	float getWidth() { return sprite.getLocalBounds().width * sprite.getScale().x + accidentalWidth; }
	
	void setAccidentalWidth(float accidentalWidth);
};

