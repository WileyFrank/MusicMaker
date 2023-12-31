#pragma once
#include "SheetMusicElement.h"
#include "SheetMusicNote.h"
#include "../../Music/MusicUtilities.h"
#include <vector>
#include <map>


class SheetMusicMeasure : public SheetMusicElement
{
private:
	float x, y, width, staffHeight;
	float quarterValue; //Beats per quarter note

	//Initialized in constructor
	float noteGap = 0;

	int noteCount;

	KeySignature keySignature;
	TimeSignature timeSignature;
	Clef clef;
	std::map<float, std::vector<Note>> notes;
	std::map<float, std::vector<SheetMusicNote*>> sheetNotes;

	//This doesn't hold the actual notes, just the chord they compose
	std::map<float, ChordType> chords;

	float hoverBeat = -1.0f;

	sf::Color staffColor, noteColor;

	//used to ensure that a measure is properly filled with notes and rests
	void checkMeasure();


	Note getRest(float beat, bool dotted = false);
	Note getLongestRest(float beat, bool dotted = false);
	void loadSheetNotes();

public:
	SheetMusicMeasure();
	SheetMusicMeasure(float x, float y, float staffHeight, Clef clef, TimeSignature timeSignature, KeySignature keySignature = KeySignature());

	~SheetMusicMeasure();

	void setupStaff(float x, float y, float staffHeight, Clef clef);

	float addNote(Note note, float beat);
	std::vector<Note> getNotes(float beat);
	std::vector<Note> getPlaying(float beat);

	void addRests();

	void draw() override;
	std::pair<sf::Vector2f, sf::Vector2f> getHoverArea() override;
	RenderObject& getHoverObject() override;

	virtual void setHover(bool hover) override;

	void setWindow(sf::RenderWindow* window);

	void updateColors();
	void reload();

	void update() override;

	void hoverUpdate();
	void unhoverUpdate();

	void hoverAction();
	void unhoverAction();

	//getters and setters
	float getWidth();

	void setStaffColor(sf::Color color);
	void setNoteColor(sf::Color color);
	void setHoverColor(sf::Color color);
	void setHeight(float height) { this->staffHeight = height; }
	void setX(float x) { this->x = x; }
	void setY(float y) { this->y = y; }
	void setKey(KeySignature key) 
	{ 
		this->keySignature = key;
	}


};

