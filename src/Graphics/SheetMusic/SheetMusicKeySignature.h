#pragma once

#include <vector>

#include "SFML/Graphics.hpp"

#include "SheetMusicElement.h"
#include "SheetMusicAccidental.h"

#include "../../Music/MusicUtilities.h"


class SheetMusicKeySignature : public SheetMusicElement
{
private: 
	float staffY, staffX, staffHeight, scaleFactor, x;

	std::vector<SheetMusicAccidental*> accidentals;
	std::vector<std::pair<PitchEnum, Accidental>> accidentalPairs;

	KeySignature key;
	Clef clef;

	sf::Color color;
	sf::Color hoverColor;
	
	void loadKeySignature();
	void addAccidental(std::pair<PitchEnum, Accidental> pitch);

public:
	SheetMusicKeySignature() :staffX(0), staffY(0), staffHeight(0), scaleFactor(0.5), key(), clef(TrebleClef), x(0) {};
	SheetMusicKeySignature(float staffX, float staffY, float staffHeight, KeySignature key, Clef clef, sf::Color color = sf::Color(255,255,255));

	void render() override;
	void update() override;
	void draw() override;
	void setWindow(sf::RenderWindow* window) override;
	std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() override;
	RenderObject& getHoverObject() override;

	void hoverUpdate();
	void hoverAction();
	void unhoverUpdate();
	void unhoverAction();

	float getWidth();
	void setColor(sf::Color color);
	void setHoverColor(sf::Color color, bool over = false);

	void drawBoundingBox(); // DEBUGGING
};

