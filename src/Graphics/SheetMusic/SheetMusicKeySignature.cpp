#include "SheetMusicKeySignature.h"

void SheetMusicKeySignature::loadKeySignature()
{
	PitchEnum notePattern[7];

	Accidental acc = Natural;

	for (auto& pitch : key.key)
	{
		if (pitch.second != Natural)
		{
			acc = pitch.second;
		}
	}

	switch (acc)
	{
	case Natural:
	case Sharp: 
		notePattern[0] = NoteF;
		notePattern[1] = NoteC;
		notePattern[2] = NoteG;
		notePattern[3] = NoteD;
		notePattern[4] = NoteA;
		notePattern[5] = NoteE;
		notePattern[6] = NoteB;
		break;
	case Flat: 
		notePattern[0] = NoteB;
		notePattern[1] = NoteE;
		notePattern[2] = NoteA;
		notePattern[3] = NoteD;
		notePattern[4] = NoteG;
		notePattern[5] = NoteC;
		notePattern[6] = NoteF;
		break;
	}

	for (auto& pitch : notePattern)
	{
		if (key.key[pitch] != Natural)
		{
			addAccidental(std::make_pair(pitch, key.key[pitch]));
		}
	}
}

void SheetMusicKeySignature::addAccidental(std::pair<PitchEnum, Accidental> pitch)
{
	accidentalPairs.push_back(pitch);
	//get the distance from the top of the staff

	int distance;
	int octave = 8;
	float positionY = 0.0f;


	SheetMusicAccidental* newAccidental;

	PitchEnum topNote = NoteG;
	float distanceToTop = 0.0f; //the distance to the top note from the top of the staff

	switch (clef)
	{
	case TrebleClef:
		

		switch (pitch.second)
		{
		case Sharp:
			topNote = NoteG;
			distanceToTop = -0.125f;
			break;

		case Flat:
			topNote = NoteE;
			distanceToTop = 0.125f;
			break;
		}
		distance = MusicUtilities::getSemitoneDistance(Pitch({ pitch.first, octave }), Pitch({ topNote, 5 }));


		while (distance < 0)
		{
			octave--;
			distance = MusicUtilities::getSemitoneDistance(Pitch({ pitch.first, octave }), Pitch({ topNote, 5 }));
		}

		distance = MusicUtilities::getNotesFromMiddleC(Pitch({ pitch.first, octave })) - MusicUtilities::getNotesFromMiddleC(Pitch({ topNote, 5 }));
		
		positionY = (-0.125f * distance) + distanceToTop;

		newAccidental = new SheetMusicAccidental(staffY, staffHeight, staffX, positionY, pitch.second);

		accidentals.push_back(newAccidental);
		staffX += newAccidental->getWidth();
		break;

	case BassClef:
		switch (pitch.second)
		{
		case Sharp:
			topNote = NoteG;
			distanceToTop = 0.125f;
			break;

		case Flat:
			topNote = NoteE;
			distanceToTop = 0.375f;
			break;
		}
		distance = MusicUtilities::getSemitoneDistance(Pitch({ pitch.first, octave }), Pitch({ topNote, 5 }));


		while (distance < 0)
		{
			octave--;
			distance = MusicUtilities::getSemitoneDistance(Pitch({ pitch.first, octave }), Pitch({ topNote, 5 }));
		}

		distance = MusicUtilities::getNotesFromMiddleC(Pitch({ pitch.first, octave })) - MusicUtilities::getNotesFromMiddleC(Pitch({ topNote, 5 }));

		positionY = (-0.125f * distance) + distanceToTop;

		newAccidental = new SheetMusicAccidental(staffY, staffHeight, staffX, positionY, pitch.second);

		accidentals.push_back(newAccidental);
		staffX += newAccidental->getWidth() / 2;
		break;
	}
}

SheetMusicKeySignature::SheetMusicKeySignature(float staffX, float staffY, float staffHeight, KeySignature key, Clef clef, sf::Color color)
	:staffX(staffX), staffY(staffY), staffHeight(staffHeight), key(key), scaleFactor(0.5), clef(clef), x(staffX), color(color), hoverColor(color)
{
	this->runHoverAction = true;
	loadKeySignature();
	setColor(color);
}

void SheetMusicKeySignature::render() 
{
	update();
	draw();
}

void SheetMusicKeySignature::update() 
{
	if (this->hover)
	{
		hoverUpdate();
		unhover = true;
		return;
	}
	if (this->unhover)
	{
		unhover = false;
		unhoverUpdate();
	}
}

void SheetMusicKeySignature::draw()
{
	for (auto& acc : accidentals)
	{
		acc->draw();
	}
	for (auto& object : objects)
	{
		object->draw();
	}
}

void SheetMusicKeySignature::setWindow(sf::RenderWindow* window)
{
	this->window = window;

	for (auto& acc : accidentals)
	{
		acc->setWindow(window);
	}
}

std::pair<sf::Vector2f, sf::Vector2f> SheetMusicKeySignature::getHoverArea()
{
	if (accidentals.size() > 0)
	{
		float topRight = accidentals.front()->getHoverArea().first.x;

		sf::Vector2f position(topRight, staffY);

		sf::Vector2f size(this->getWidth(), staffHeight);
		return std::pair<sf::Vector2f, sf::Vector2f>(position, size);
	}

	return std::pair<sf::Vector2f, sf::Vector2f>({ 0.0f,0.0f }, { 0.0f,0.0f });
}

RenderObject& SheetMusicKeySignature::getHoverObject()
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);

	if (GUIUtilities::positionInBounds(mousePosition, this->getHoverArea()))
	{
		return *this;
	}

	return *GUIUtilities::getEmptyRenderObject();
}

void SheetMusicKeySignature::hoverUpdate()
{
	if (!unhover)
	{
		this->hoverStart = std::chrono::high_resolution_clock::now();
		for (auto& acc : accidentals)
		{
			acc->setColor(hoverColor);
		}
	}

	auto timeNow = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(timeNow - hoverStart);

	if (duration.count() >= 300 && runHoverAction)
	{
		hoverAction();
		runHoverAction = false;
	}
}

void SheetMusicKeySignature::hoverAction()
{
	auto mouse = sf::Mouse::getPosition(*window);
	auto noteBounds = getHoverArea();

	float maxY = noteBounds.first.y;

	float height = 60.0f;

	hoverPanel = addPanel(noteBounds.first.x, maxY - height, 120.0f, height, sf::Color(11, 0, 45), sf::Color(31, 24, 96), 2);
	hoverPanel->addText("Key: " + key.name, 16, sf::Color(190, 188, 216));

	std::string accidentalString;

	if (accidentals.size() > 0)
	{
		for (auto& acc : accidentalPairs)
		{
			accidentalString += MusicUtilities::getNoteString(acc.first) + ", ";
		}

		accidentalString = accidentalString.substr(0, accidentalString.length() - 2);

		if (accidentalString.length() > 8)
		{
			accidentalString = accidentalString.substr(0, 8) +  "\n" + accidentalString.substr(9, accidentalString.length());
		}

		switch (accidentalPairs[0].second)
		{
		case Flat:
			accidentalString = "Flats: " + accidentalString;
			break;
		case Sharp:
			accidentalString = "Sharps: " + accidentalString;
			break;
		}


		hoverPanel->addText(accidentalString, 16, sf::Color(190, 188, 216));

	}
}

void SheetMusicKeySignature::unhoverUpdate()
{
	for (auto& acc : accidentals)
	{
		acc->setColor(color);
	}
	unhoverAction();
}

void SheetMusicKeySignature::unhoverAction()
{
	runHoverAction = true;
	auto it = std::find(objects.begin(), objects.end(), hoverPanel);
	if (it != objects.end()) {
		objects.erase(it);
	}
}

float SheetMusicKeySignature::getWidth()
{
	float width = 0;
	for (auto accidental : accidentals)
	{
		width += accidental->getWidth();
	}

	return width;
}

void SheetMusicKeySignature::setColor(sf::Color color)
{
	if (this->hoverColor == this->color)
	{
		this->hoverColor = color;
	}
	this->color = color;

	for (auto& acc : accidentals)
	{
		acc->setColor(color);
	}
}

void SheetMusicKeySignature::setHoverColor(sf::Color color, bool over)
{
	if (over)
	{
		this->hoverColor = color;
	}
	else if (hoverColor == this->color)
	{
		this->hoverColor = color;
	}
}

void SheetMusicKeySignature::drawBoundingBox()
{
	auto bounds = getHoverArea();
	sf::RectangleShape rect(bounds.second);
	rect.setPosition(bounds.first);
	rect.setOutlineColor(sf::Color(255, 0, 0));
	rect.setFillColor(sf::Color(0, 0, 0, 0));
	rect.setOutlineThickness(2);

	window->draw(rect);
}
