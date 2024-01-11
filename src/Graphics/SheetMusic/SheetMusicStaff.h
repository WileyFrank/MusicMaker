#pragma once

#include <vector>
#include "SFML/Graphics.hpp"
#include "SheetMusicElement.h"
#include "SheetMusicClef.h"
#include "SheetMusicMeasure.h"
#include "SheetMusicKeySignature.h"
#include "SheetMusicTimeSignature.h"
#include "../../Music/MusicUtilities.h"

//A staff of music containing:
//Measures which contain Notes -> (Note, accidental, additional (dots, accents, etc.)
//Clef - Type and symbol
//TimeSignature - Num, Denom, and Symbols for each
//KeySignature - A collection of accidentals

// |-|-|-|-|-|-|-|-|-|-|-|-| POSITIONING |-|-|-|-|-|-|-|-|-|-|-|-|-|-|
/*	All positions are relative to the position of the Staff
*	
*	Position of the top bar : 0.0f
*	Position of the bottom  : 1.0f
*	Distance between bars	: 0.25f 
*	-Though there are 5 bars, there are 4 gaps-
* 
*	For notes the distance from the top bar is calculated
*	The Positioning point, the center of a note, or note of a clef, 
*	is set to the origin and the position is set to the proper not position
*/

// |-|-|-|-|-|-|-|-|-|-|-|-| Notes |-|-|-|-|-|-|-|-|-|-|-|-|-|-|
/* Notes can be added in one of 3 ways
*  
*	- By passed in measure
*	- By adding notes to an existing staff measure
*	- By using abstacted methods such as addNote which will use
*	  one of the above methods to add a note at a provided beat
*/

// |-|-|-|-|-|-|-|-|-|-|-|-| Audio |-|-|-|-|-|-|-|-|-|-|-|-|-|-|
/*
* Audio will be handled by the mixer,
* which is to be passed in using setMixer()
*
*/

class SheetMusicStaff : public SheetMusicElement
{
private:
	
	float measureStart;
	float measureGap = 0.0f;
	float currentMeasure = 0.0f;


	std::vector<sf::RectangleShape*> lines;
	std::vector<sf::RectangleShape*> bars;
	SheetMusicClef clef;
	Clef clefType;

	KeySignature keySignature;
	SheetMusicKeySignature sheetMusicKeySignature;

	TimeSignature timeSignature;
	SheetMusicTimeSignature sheetMusicTimeSignature;


	sf::Color staffColor, clefColor, noteColor, hoverColor, noteHoverColor, placeholderColor;

	std::vector<SheetMusicMeasure*> measures;
 
	void GenerateStaffLines();
	float getStaffPosition(Pitch note);
	void addBar(float xPosition);

public:
	SheetMusicStaff();
	SheetMusicStaff(float x, float y, float width, float height, Clef clefType, 
		KeySignature key = KeySignature(), TimeSignature timeSignature = TimeSignature());

	~SheetMusicStaff();

	SheetMusicMeasure* addMeasure(SheetMusicMeasure* measure);
	SheetMusicMeasure* addMeasure();

	float addNote(Note note, float beat);

	void draw() override;
	void setWindow(sf::RenderWindow* window) override;
	std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() override;

	RenderObject& getHoverObject() override;

	virtual void setHover(bool hover) override;

	virtual void render() override;
	virtual void update() override;



	void hoverUpdate();
	void unhoverUpdate();


	void colorUpdate();

	void setColor(sf::Color color);
	void setNoteColor(sf::Color color);
	void setClefColor(sf::Color color);
	void setHoverColor(sf::Color color);
	void setNoteHoverColor(sf::Color color);

	SheetMusicClef* getSheetMusicClef() { return &clef; }
	SheetMusicKeySignature* getSheetMusicKeySignature() { return &sheetMusicKeySignature; }
	SheetMusicTimeSignature* getSheetMusicTimeSignature() { return &sheetMusicTimeSignature; }


};

