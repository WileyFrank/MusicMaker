#include "SheetMusicStaff.h"
#include "../GUI/GUIUtilities.h"
#include <algorithm>
#include <cmath>
#include <new>

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

	rowSpacing = this->height * 1.125f;
	updateMeasureStarts();

}

void SheetMusicStaff::updateMeasureStarts()
{
	const float timeSigWidth = hideTimeSignature_ ? 0.0f : sheetMusicTimeSignature.getWidth();
	measureStart =
		clef.getWidth() +
		sheetMusicKeySignature.getWidth() +
		timeSigWidth +
		(this->height / 2.0f);
	continuationMeasureStart =
		clef.getWidth() +
		(this->height / 6.0f) +
		sheetMusicKeySignature.getWidth() +
		(this->height / 2.0f);
}

SheetMusicStaff::SheetMusicStaff(
	const RectSpec& rectSpec,
	const MarginSpec& marginSpec,
	Clef clefType,
	KeySignature key,
	TimeSignature timeSignature
)
	: SheetMusicStaff(0.0f, 0.0f, 0.0f, 0.0f, clefType, key, timeSignature)
{
	setRectSpec(rectSpec);
	setMarginSpec(marginSpec);
}

SheetMusicStaff::~SheetMusicStaff()
{
	for (auto shape : lines)
	{
		delete shape;
	}
	lines.clear();
	for (auto shape : bars)
	{
		delete shape;
	}
	bars.clear();
	for (auto* measure : measures)
	{
		delete measure;
	}
	measures.clear();
	clearWrappedSymbols();
}

SheetMusicMeasure* SheetMusicStaff::addMeasure(SheetMusicMeasure* measure) {
	SheetMusicMeasure* newMeasure = new SheetMusicMeasure(measure);
	newMeasure->setNoteColor(noteColor);
	newMeasure->setStaffColor(staffColor);
	newMeasure->setWindow(this->window);
	measures.push_back(newMeasure);
	relayoutMeasuresWithWrap();
	colorUpdate();

	return newMeasure;
}

SheetMusicMeasure* SheetMusicStaff::addMeasure()
{
	SheetMusicMeasure* newMeasure = new SheetMusicMeasure(0.0f, 0.0f, height, clef.getClef(), timeSignature, keySignature);

	newMeasure->setNoteColor(noteColor);
	newMeasure->setStaffColor(staffColor);
	newMeasure->setWindow(this->window);
	measures.push_back(newMeasure);
	relayoutMeasuresWithWrap();
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
	measures[measure]->clearRests();
	measures[measure]->addNote(note,placement);
	measures[measure]->addRests();
	measures[measure]->reload();
	relayoutMeasuresWithWrap();
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
	if (!hideTimeSignature_)
	{
		sheetMusicTimeSignature.draw();
	}
	for (auto* wrappedClef : wrappedClefs)
	{
		if (wrappedClef != nullptr)
		{
			wrappedClef->draw();
		}
	}
	for (auto* wrappedKeySignature : wrappedKeySignatures)
	{
		if (wrappedKeySignature != nullptr)
		{
			wrappedKeySignature->draw();
		}
	}
}

void SheetMusicStaff::resolveLayout(const sf::FloatRect& parentRect)
{
	RenderObject::resolveLayout(parentRect);
	const sf::FloatRect pixelRect = getResolvedRect();

	x = pixelRect.left;
	y = pixelRect.top;
	width = pixelRect.width;
	height = pixelRect.height;

	// Rebuild clef/key/time geometry for the new staff bounds.
	clef.~SheetMusicClef();
	new (&clef) SheetMusicClef(x, y, width, height, clefType);

	sheetMusicKeySignature.~SheetMusicKeySignature();
	new (&sheetMusicKeySignature) SheetMusicKeySignature((x + clef.getWidth() + height / 6.0f), y, height, keySignature, clefType);

	sheetMusicTimeSignature.~SheetMusicTimeSignature();
	new (&sheetMusicTimeSignature) SheetMusicTimeSignature((x + clef.getWidth() + sheetMusicKeySignature.getWidth() + height / 3.0f), y, height, timeSignature);

	if (this->window != nullptr)
	{
		clef.setWindow(this->window);
		sheetMusicKeySignature.setWindow(this->window);
		sheetMusicTimeSignature.setWindow(this->window);
	}

	rowSpacing = this->height * 1.125f;
	updateMeasureStarts();
	relayoutMeasuresWithWrap();

	colorUpdate();
}

void SheetMusicStaff::setWindow(sf::RenderWindow* window)
{
	this->window = window;
	clef.setWindow(window);
	sheetMusicKeySignature.setWindow(window);
	sheetMusicTimeSignature.setWindow(window);
	for (auto* wrappedClef : wrappedClefs)
	{
		if (wrappedClef != nullptr)
		{
			wrappedClef->setWindow(window);
		}
	}
	for (auto* wrappedKeySignature : wrappedKeySignatures)
	{
		if (wrappedKeySignature != nullptr)
		{
			wrappedKeySignature->setWindow(window);
		}
	}

	for (auto& measure : measures)
	{
		measure->setWindow(window);
	}
}

std::pair<sf::Vector2f, sf::Vector2f> SheetMusicStaff::getHoverArea()
{
	sf::Vector2f position(x, y);
	sf::Vector2f size(0.0f, 0.0f);

	return std::pair<sf::Vector2f, sf::Vector2f>(position, size);
}

RenderObject& SheetMusicStaff::getHoverObject()
{
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


	if (!hideTimeSignature_)
	{
		if (sheetMusicTimeSignature.getHoverObject().getType() != EmptyRenderObject)
		{
			return sheetMusicTimeSignature.getHoverObject();
		}
	}
	for (auto* wrappedClef : wrappedClefs)
	{
		if (wrappedClef != nullptr && wrappedClef->getHoverObject().getType() != EmptyRenderObject)
		{
			return wrappedClef->getHoverObject();
		}
	}
	for (auto* wrappedKeySignature : wrappedKeySignatures)
	{
		if (wrappedKeySignature != nullptr && wrappedKeySignature->getHoverObject().getType() != EmptyRenderObject)
		{
			return wrappedKeySignature->getHoverObject();
		}
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
	if (!hideTimeSignature_)
	{
		sheetMusicTimeSignature.setHover(hover);
	}
	for (auto* wrappedClef : wrappedClefs)
	{
		if (wrappedClef != nullptr)
		{
			wrappedClef->setHover(hover);
		}
	}
	for (auto* wrappedKeySignature : wrappedKeySignatures)
	{
		if (wrappedKeySignature != nullptr)
		{
			wrappedKeySignature->setHover(hover);
		}
	}

	for (auto& measure : measures)
	{
		measure->setHover(hover);
	}
}

void SheetMusicStaff::render()
{
	clef.update();
	sheetMusicKeySignature.update();
	if (!hideTimeSignature_)
	{
		sheetMusicTimeSignature.update();
	}
	for (auto* wrappedClef : wrappedClefs)
	{
		if (wrappedClef != nullptr)
		{
			wrappedClef->update();
		}
	}
	for (auto* wrappedKeySignature : wrappedKeySignatures)
	{
		if (wrappedKeySignature != nullptr)
		{
			wrappedKeySignature->update();
		}
	}


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
	for (auto shape : bars)
	{
		shape->setFillColor(color);
	}
	for (auto& measure : measures)
	{
		measure->setStaffColor(color);
	}
	
	clef.setColor(color);
	if (!hideTimeSignature_)
	{
		sheetMusicTimeSignature.setColor(color);
	}
	for (auto* wrappedClef : wrappedClefs)
	{
		if (wrappedClef != nullptr)
		{
			wrappedClef->setColor(color);
		}
	}
}

void SheetMusicStaff::setNoteColor(sf::Color color)
{
	for (auto& measure : measures)
	{
		measure->setNoteColor(color);
	}

	noteColor = color;
	sheetMusicKeySignature.setColor(color);
	for (auto* wrappedKeySignature : wrappedKeySignatures)
	{
		if (wrappedKeySignature != nullptr)
		{
			wrappedKeySignature->setColor(color);
		}
	}
}

void SheetMusicStaff::setClefColor(sf::Color color)
{
	clefColor = color;
	clef.setColor(color);
	for (auto* wrappedClef : wrappedClefs)
	{
		if (wrappedClef != nullptr)
		{
			wrappedClef->setColor(color);
		}
	}
}

void SheetMusicStaff::setHoverColor(sf::Color color)
{
	this->hoverColor = color;
	sheetMusicKeySignature.setHoverColor(color);
	if (!hideTimeSignature_)
	{
		sheetMusicTimeSignature.setHoverColor(color);
	}
	clef.setHoverColor(color);
	for (auto* wrappedKeySignature : wrappedKeySignatures)
	{
		if (wrappedKeySignature != nullptr)
		{
			wrappedKeySignature->setHoverColor(color);
		}
	}
	for (auto* wrappedClef : wrappedClefs)
	{
		if (wrappedClef != nullptr)
		{
			wrappedClef->setHoverColor(color);
		}
	}
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
	clearStaffGeometry();
	addStaffRow(0);
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

void SheetMusicStaff::clearStaffGeometry()
{
	for (auto* shape : lines)
	{
		delete shape;
	}
	lines.clear();

	for (auto* shape : bars)
	{
		delete shape;
	}
	bars.clear();
}

void SheetMusicStaff::clearWrappedSymbols()
{
	for (auto* wrappedClef : wrappedClefs)
	{
		delete wrappedClef;
	}
	wrappedClefs.clear();

	for (auto* wrappedKeySignature : wrappedKeySignatures)
	{
		delete wrappedKeySignature;
	}
	wrappedKeySignatures.clear();
}

float SheetMusicStaff::getRowTop(int rowIndex) const
{
	const float rowStep = height + rowSpacing;
	return y + static_cast<float>(rowIndex) * rowStep;
}

float SheetMusicStaff::getRowPrefixWidth(int rowIndex) const
{
	return rowIndex == 0 ? measureStart : continuationMeasureStart;
}

void SheetMusicStaff::addStaffRow(int rowIndex)
{
	int lineWidth = static_cast<int>(height / 40.0f);
	if (lineWidth < 1)
	{
		lineWidth = 1;
	}

	const float lineThickness = static_cast<float>(lineWidth);
	const float spaceWidth = height - lineThickness * 5.0f;
	const float rowTop = getRowTop(rowIndex);

	for (int i = 0; i < 5; i++)
	{
		const float lineY = (spaceWidth / 4.0f + lineThickness) * i + rowTop;
		auto* newShape = new sf::RectangleShape(sf::Vector2f(width, lineThickness));
		newShape->setPosition(sf::Vector2f(this->x, lineY));
		newShape->setFillColor(staffColor);
		lines.push_back(newShape);
	}

	int barWidth = static_cast<int>(height / 20.0f);
	if (barWidth < 1)
	{
		barWidth = 1;
	}

	auto* leftBar = new sf::RectangleShape(sf::Vector2f(static_cast<float>(barWidth), this->height));
	leftBar->setPosition(this->x, rowTop);
	leftBar->setFillColor(staffColor);
	lines.push_back(leftBar);
}

void SheetMusicStaff::relayoutMeasuresWithWrap()
{
	clearStaffGeometry();
	clearWrappedSymbols();
	if (height <= 0.0f || width <= 0.0f)
	{
		return;
	}

	const float preMeasureGap = height / 4.0f;
	const float barWidth = std::max(1.0f, height / 20.0f);
	const float probeRowTop = getRowTop(0);
	const Clef activeClef = clef.getClef();
	const bool requiresMeasureRebuild =
		(!hasMeasuredWidths) ||
		(std::fabs(lastMeasuredHeight - height) > 0.001f);

	// Precompute widths only when staff height changed (or first run).
	// Width-only resizes can reuse existing note geometry widths.
	std::vector<float> measureWidths(measures.size(), 0.0f);
	for (size_t i = 0; i < measures.size(); ++i)
	{
		SheetMusicMeasure* measure = measures[i];
		if (measure == nullptr)
		{
			continue;
		}

		if (requiresMeasureRebuild)
		{
			measure->setupStaff(0.0f, probeRowTop, height, activeClef);
			measure->setWindow(this->window);
			measureWidths[i] = measure->getWidth();
		}
		else
		{
			measureWidths[i] = measure->getWidth();
		}
	}
	hasMeasuredWidths = true;
	lastMeasuredHeight = height;

	int currentRow = 0;
	addStaffRow(currentRow);

	float runningX = 0.0f;
	float rowUsableWidth = width - getRowPrefixWidth(currentRow);
	if (rowUsableWidth < 0.0f)
	{
		rowUsableWidth = 0.0f;
	}

	for (size_t i = 0; i < measures.size(); ++i)
	{
		SheetMusicMeasure* measure = measures[i];
		if (measure == nullptr)
		{
			continue;
		}

		const float measureWidth = measureWidths[i];
		const float requiredWidth = preMeasureGap + measureWidth;
		const bool exceedsRow = runningX > 0.0f && (runningX + requiredWidth > rowUsableWidth);

		if (exceedsRow)
		{
			currentRow++;
			addStaffRow(currentRow);
			const float rowTop = getRowTop(currentRow);
			auto* wrappedClef = new SheetMusicClef(this->x, rowTop, width, height, clefType);
			wrappedClef->setColor(clefColor);
			wrappedClef->setHoverColor(hoverColor);
			wrappedClef->setWindow(this->window);
			wrappedClefs.push_back(wrappedClef);

			auto* wrappedKey = new SheetMusicKeySignature(
				this->x + wrappedClef->getWidth() + (height / 6.0f),
				rowTop,
				height,
				keySignature,
				clefType
			);
			wrappedKey->setColor(noteColor);
			wrappedKey->setHoverColor(hoverColor);
			wrappedKey->setWindow(this->window);
			wrappedKeySignatures.push_back(wrappedKey);

			runningX = 0.0f;
			rowUsableWidth = width - getRowPrefixWidth(currentRow);
			if (rowUsableWidth < 0.0f)
			{
				rowUsableWidth = 0.0f;
			}
		}

		const float rowPrefix = getRowPrefixWidth(currentRow);
		const float rowTop = getRowTop(currentRow);

		if (runningX > 0.0f)
		{
			auto* divider = new sf::RectangleShape(sf::Vector2f(barWidth, height));
			divider->setPosition(sf::Vector2f(this->x + rowPrefix + runningX, rowTop));
			divider->setFillColor(staffColor);
			bars.push_back(divider);
		}

		const float measureX = this->x + rowPrefix + runningX + preMeasureGap;
		measure->setupStaff(measureX, rowTop, height, activeClef);
		measure->setWindow(this->window);

		runningX += requiredWidth + measureGap;
	}

	currentMeasure = runningX;
}
