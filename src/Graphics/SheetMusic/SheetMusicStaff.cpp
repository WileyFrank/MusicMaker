#include "SheetMusicStaff.h"
#include "../GUI/GUIUtilities.h"

//public
SheetMusicStaff::SheetMusicStaff()
	: clefType(TrebleClef), clef(x, y, width, height, clefType)
{
	this->x = 0;
	this->y = 0;
	this->width = 100;
	this->height = 300;
	GenerateStaffLines();
}

SheetMusicStaff::SheetMusicStaff(float x, float y, float width, float height, Clef clefType, KeySignature key, TimeSignature timeSignature)
	:measureGap(height / 3.0f),
	clefType(clefType), clef(x, y, width, height, clefType), measureStart(0.0f),
	keySignature(key), sheetMusicKeySignature((x + clef.getWidth() + height / 6.0f), y, height, key, clefType),
	timeSignature(timeSignature), sheetMusicTimeSignature((x + clef.getWidth() + sheetMusicKeySignature.getWidth() + height / 3.0f), y, height, timeSignature)
{
	//this->clef = SheetMusicClef(x, y, width, height, clefType);

	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;

	GenerateStaffLines();

	auto clefWidth = clef.getWidth();
	clefWidth = sheetMusicKeySignature.getWidth();
	clefWidth = sheetMusicTimeSignature.getWidth();

	measureStart = (clef.getWidth()) +
		(sheetMusicKeySignature.getWidth()) +
		(sheetMusicTimeSignature.getWidth()) +
		(this->height / 2);

}

SheetMusicStaff::~SheetMusicStaff()
{
	for (auto shape : lines)
	{
		delete shape;
	}
	lines.clear();
}

SheetMusicMeasure* SheetMusicStaff::addMeasure(SheetMusicMeasure* measure) {
	currentMeasure = 0.0f;

	// Sum the widths of existing measures
	for (auto& existingMeasure : measures) {
		currentMeasure += existingMeasure->getWidth() + measureGap;
	}

	// Create a new bar
	auto newRect = new sf::RectangleShape(sf::Vector2f(height / 20, height));
	newRect->setPosition(sf::Vector2f(currentMeasure + this->x + measureStart, y));
	newRect->setFillColor(staffColor);
	bars.push_back(newRect);

	currentMeasure += height / 5;

	// Create a copy of the measure
	SheetMusicMeasure* newMeasure = new SheetMusicMeasure(measure); // Copying the measure
	newMeasure->setupStaff(currentMeasure + measureStart + this->x, this->y, height, clef.getClef());
	newMeasure->setNoteColor(noteColor);
	newMeasure->setStaffColor(staffColor);
	newMeasure->setWindow(this->window);
	measures.push_back(newMeasure);
	currentMeasure += newMeasure->getWidth();

	return newMeasure; // Return the pointer to the new measure
}

SheetMusicMeasure* SheetMusicStaff::addMeasure()
{
	currentMeasure = 0.0f;

	for (auto& measure : measures)
	{
		currentMeasure += measure->getWidth() + measureGap;
	}

	if (measures.size() > 0)
	{
		auto newRect = new sf::RectangleShape(sf::Vector2f(height / 20, height));

		newRect->setPosition(sf::Vector2f(currentMeasure + measureStart + this->x, y));
		newRect->setFillColor(staffColor);
		bars.push_back(newRect);
	}

	currentMeasure += height / 4;




	SheetMusicMeasure* newMeasure = new SheetMusicMeasure(currentMeasure + measureStart + this->x, this->y, height, clef.getClef(), timeSignature, keySignature);

	newMeasure->setNoteColor(noteColor);
	newMeasure->setStaffColor(staffColor);
	newMeasure->setWindow(this->window);
	measures.push_back(newMeasure);
	colorUpdate();

	return newMeasure;
}

float SheetMusicStaff::addNote(Note note, float beat)
{
	int measure = (int)beat / timeSignature.denominator;
	if (measures.size() <= measure)
	{	
		SheetMusicMeasure* newMeasure = this->addMeasure();
		while (measures.size() <= measure)
		{
			newMeasure->addRests();
			newMeasure->reload();
			newMeasure = this->addMeasure();
		}


	}

	float placement = fmod(beat, (float)timeSignature.denominator);
	float measureWidth = measures[measure]->getWidth();
	measures[measure]->clearRests();
	measures[measure]->addNote(note,placement);
	measures[measure]->addRests();
	measures[measure]->reload();
	//If affecting a measure with more measures after it, update the later ones
	if (measure != measures.size() - 1)
	{
		float difference = measures[measure]->getWidth() - measureWidth;
		if (difference)
		{
			for (int i = measure + 1; i < measures.size(); i++)
			{
				measures[i]->moveX(difference);
				int j = i - 1;
				bars[j]->setPosition(bars[j]->getPosition().x + difference, bars[j]->getPosition().y);
			}
		}
	}
	colorUpdate();
	return beat + MusicUtilities::getBeats(timeSignature, note.value);;
}

void SheetMusicStaff::draw()
{
	//Draw the lines of the staff
	for (auto& shape : lines)
	{
		window->draw(*shape);
	}

	for (auto& bar : bars)
	{
		window->draw(*bar);
	}

	for (auto& measure : measures)
	{
		measure->draw();
	}

	clef.draw();
	sheetMusicKeySignature.draw();
	sheetMusicTimeSignature.draw();
}

void SheetMusicStaff::setWindow(sf::RenderWindow* window)
{
	this->window = window;
	clef.setWindow(window);
	sheetMusicKeySignature.setWindow(window);
	sheetMusicTimeSignature.setWindow(window);
}

std::pair<sf::Vector2f, sf::Vector2f> SheetMusicStaff::getHoverArea()
{
	sf::Vector2f position(x, y);
	sf::Vector2f size(0.0f, 0.0f);

	return std::pair<sf::Vector2f, sf::Vector2f>(position, size);
}

RenderObject& SheetMusicStaff::getHoverObject()
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);


	//clef, key, time

	if (clef.getHoverObject().getType() != EmptyRenderObject)
	{
		return clef.getHoverObject();
	}
	
	auto& sheetMusicHover = sheetMusicKeySignature.getHoverObject();
	if (sheetMusicHover.getType() != EmptyRenderObject)
	{
		return sheetMusicHover;
	}


	if (sheetMusicTimeSignature.getHoverObject().getType() != EmptyRenderObject)
	{
		return sheetMusicTimeSignature.getHoverObject();
	}
	for (auto& measure : measures)
	{
		if (measure->getHoverObject().getType() != EmptyRenderObject)
		{
			return measure->getHoverObject();
		}
	}


	RenderObject* empty = GUIUtilities::getEmptyRenderObject();
	return *empty;
}

void SheetMusicStaff::setHover(bool hover)
{
	this->hover = hover;

	clef.setHover(hover);
	sheetMusicKeySignature.setHover(hover);
	sheetMusicTimeSignature.setHover(hover);

	for (auto& measure : measures)
	{
		measure->setHover(hover);
	}
}

void SheetMusicStaff::render()
{
	clef.update();
	sheetMusicKeySignature.update();
	sheetMusicTimeSignature.update();


	for (auto& measure : measures)
	{
		measure->update();
	}
	update();
	draw();
}

void SheetMusicStaff::update()
{
	if (this->hover)
	{
		//hoverUpdate();
		unhover = true;
		return;
	}
	if (this->unhover)
	{
		unhover = false;
		//unhoverUpdate();
	}
	draw();
}

void SheetMusicStaff::hoverUpdate()
{
	if (!unhover)
	{
		placeholderColor = staffColor;

		setColor(hoverColor);
	}
}

void SheetMusicStaff::unhoverUpdate()
{
	staffColor = placeholderColor;

	setColor(staffColor);
}

void SheetMusicStaff::colorUpdate()
{
	setColor(staffColor);
	setNoteColor(noteColor);
	setNoteHoverColor(noteHoverColor);
	setHoverColor(hoverColor);
	setClefColor(clefColor);
}

void SheetMusicStaff::setColor(sf::Color color)
{
	staffColor = color;
	for (auto shape : lines)
	{
		shape->setFillColor(color);
	}
	for (auto& measure : measures)
	{
		measure->setStaffColor(color);
	}
	
	clef.setColor(color);
	sheetMusicTimeSignature.setColor(color);
}

void SheetMusicStaff::setNoteColor(sf::Color color)
{
	for (auto& measure : measures)
	{
		measure->setNoteColor(color);
	}

	noteColor = color;
	sheetMusicKeySignature.setColor(color);
}

void SheetMusicStaff::setClefColor(sf::Color color)
{
	clefColor = color;
	clef.setColor(color);
}

void SheetMusicStaff::setHoverColor(sf::Color color)
{
	this->hoverColor = color;
	sheetMusicKeySignature.setHoverColor(color);
	sheetMusicTimeSignature.setHoverColor(color);
	clef.setHoverColor(color);
}

void SheetMusicStaff::setNoteHoverColor(sf::Color color)
{
	this->noteHoverColor = color;
	for (auto measure : measures)
	{
		measure->setHoverColor(color);
	}
}

void SheetMusicStaff::GenerateStaffLines()
{
	int lineWidth = (int)height/40;
	if (lineWidth < 1) lineWidth = 1;

	float spaceWidth = this->height - lineWidth * 5;

	float newY;

	sf::RectangleShape* newShape;
	for (int i = 0; i < 5; i++)
	{
		newY = (spaceWidth / 4 + lineWidth) * i + y;

		newShape = new sf::RectangleShape(sf::Vector2f(width, (float)lineWidth));
		newShape->setPosition(sf::Vector2f(this->x, newY));

		lines.push_back(newShape);
	}

	addBar(0);
}

//returns the position of a note in the staff
//the position is in terms of height of the staff so a note on the top bar is 0 and a note on the bottom bar is 1
//this continues to negative for higher values and > 1 for lower notes
float SheetMusicStaff::getStaffPosition(Pitch note)
{


	return 0.0f;
}

void SheetMusicStaff::addBar(float xPosition)
{
	int lineWidth = (int)height / 20;
	if (lineWidth < 1) lineWidth = 1;


	sf::RectangleShape* newShape = new sf::RectangleShape(sf::Vector2f((float)lineWidth, this->height));
	newShape->setPosition(xPosition + this->x, this->y);
	lines.push_back(newShape);
}
