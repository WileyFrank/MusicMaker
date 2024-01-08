#include "SheetMusicNote.h"

SheetMusicNote::SheetMusicNote()
	:staffX(100), staffY(100), staffHeight(100), clef(TrebleClef), C4Position(1.25f), accidental(Natural),
	notePointVertical(0), notePointHorizontal(0), halfStep(100.0f / 8), note(Note()), noteX(0)
{
	this->window = nullptr;
}

SheetMusicNote::SheetMusicNote(SheetMusicNote* old)
	:staffX(old->staffX), staffY(old->staffY), staffHeight(old->staffHeight), clef(old->clef), key(old->key), C4Position(old->C4Position),
	notePointVertical(old->notePointVertical), notePointHorizontal(old->notePointHorizontal), halfStep(100.0f / 8),
	note(old->note), noteX(old->noteX), accidental(old->accidental), SMAccidental(staffY, staffHeight, 0, 0, accidental),
	noteCount(old->noteCount)
{
	loadNote();
}

SheetMusicNote::SheetMusicNote(float x, float y, float height, Clef clef, Note note, Accidental accidental, KeySignature key)
	:staffX(x), staffY(y), staffHeight(height), clef(clef), key(key), C4Position(1.25f),
	notePointVertical(0), notePointHorizontal(0), halfStep(100.0f / 8),
	note(note), noteX(0), accidental(accidental), SMAccidental(y, height, 0, 0, accidental)
{
	this->window = nullptr;
	loadNote();
}

void SheetMusicNote::addLinesToNote(Pitch pitch)
{
	int distance = MusicUtilities::getNotesFromMiddleC(note.pitch, true);
	int lineWidth = (int)(staffHeight / 40);

	switch (clef)
	{
	case BassClef:
		if (distance > -1)
		{
			float yPos = -0.25;
			yPos *= staffHeight;
			yPos += staffY;
			int lines = (int)((float)(distance + 2) / 2);
			for (int i = 0; i < lines; i++)
			{
				sf::RectangleShape rect = sf::RectangleShape(sf::Vector2f(staffHeight / 2, (float)lineWidth));
				rect.setOrigin(sf::Vector2f(staffHeight / 4, 0));
				rect.setFillColor(staffColor);
				rect.setPosition(sf::Vector2f(staffX + accidentalWidth + sprite.getOrigin().x * sprite.getScale().x, yPos - (2 * halfStep) * i));
				extraLines.push_back(rect);
			}
		}
		else
		{
			float yPos = 1.25;
			yPos *= staffHeight;
			yPos += staffY;

			int lines = (int)(float)(distance + 9) / -2;

			for (int i = 0; i < lines; i++)
			{
				sf::RectangleShape rect = sf::RectangleShape(sf::Vector2f(staffHeight / 2, (float)lineWidth));
				rect.setOrigin(sf::Vector2f(staffHeight / 4, 0));
				rect.setFillColor(sf::Color(255, 255, 255));
				rect.setFillColor(staffColor);
				rect.setPosition(sf::Vector2f(staffX + accidentalWidth + sprite.getOrigin().x * sprite.getScale().x, yPos + (2 * halfStep) * i));
				extraLines.push_back(rect);
			}
		}
		break;
	case TrebleClef:
		if (distance > 11)
		{
			float yPos = -0.25;
			yPos *= staffHeight;
			yPos += staffY;
			int lines = (int)((float)(distance - 10) / 2);
			for (int i = 0; i < lines; i++)
			{
				sf::RectangleShape rect = sf::RectangleShape(sf::Vector2f(staffHeight / 2, (float)lineWidth));
				rect.setOrigin(sf::Vector2f(staffHeight / 4, 0));
				rect.setFillColor(staffColor);
				rect.setPosition(sf::Vector2f(staffX + accidentalWidth + sprite.getOrigin().x * sprite.getScale().x, yPos - (2 * halfStep) * i));
				extraLines.push_back(rect);
			}
		}
		else
		{
			float yPos = 1.25;
			yPos *= staffHeight;
			yPos += staffY;

			int lines = (int)(float)(distance - 2) / -2;

			for (int i = 0; i < lines; i++)
			{
				sf::RectangleShape rect = sf::RectangleShape(sf::Vector2f(staffHeight / 2, (float)lineWidth));
				rect.setOrigin(sf::Vector2f(staffHeight / 4, 0));
				rect.setFillColor(sf::Color(255, 255, 255));
				rect.setFillColor(staffColor);
				rect.setPosition(sf::Vector2f(staffX + accidentalWidth + sprite.getOrigin().x * sprite.getScale().x, yPos + (2 * halfStep) * i));
				extraLines.push_back(rect);
			}
		}
		break;
	}
}

void SheetMusicNote::draw()
{

	if (!(window == nullptr)) {
		if (extraLines.size() > 0)
		{
			for (auto& line : extraLines)
			{
				window->draw(line);
			}
		}

		window->draw(sprite);
		for (auto& object : objects)
		{
			object->render();
		}
	}
	else
	{
		std::cerr << "Window uninitialized on Sheet Music Note\n";
	}

	if (accidental != Natural)
	{
		SMAccidental.draw();
	}

}

void SheetMusicNote::setStaffColor(sf::Color color)
{
	for (auto& shape : extraLines)
	{
		shape.setFillColor(color);
	}

	staffColor = color;
}

void SheetMusicNote::render()
{
	update();
	draw();
}


void SheetMusicNote::update()
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


void SheetMusicNote::setNoteColor(sf::Color color)
{
	sprite.setColor(color);

	noteColor = color;
	SMAccidental.setColor(color);
}

void SheetMusicNote::setHoverColor(sf::Color color)
{
	hoverColor = color;
	SMAccidental.setHoverColor(color);
}

void SheetMusicNote::setAccidental(Accidental accidental)
{
	this->accidental = accidental;
}

void SheetMusicNote::hoverUpdate()
{
	if (!unhover)
	{
		runHoverAction = true;
		this->hoverStart = std::chrono::high_resolution_clock::now();
		sprite.setColor(hoverColor);
		SMAccidental.setColor(hoverColor);
	}
	//drawBoundingBox();

	auto timeNow = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(timeNow - hoverStart);

	if (duration.count() >= 150 && runHoverAction &&!overrideAction)
	{
		hoverAction();
		runHoverAction = false;
	}
}

void SheetMusicNote::unhoverUpdate()
{
	sprite.setColor(noteColor);
	SMAccidental.setColor(noteColor);

	unhoverAction();
}

void SheetMusicNote::hoverAction()
{
	auto mouse = sf::Mouse::getPosition(*window);
	auto noteBounds = getHoverArea();

	float maxY = noteBounds.first.y;

	float height = 60.0f;

	hoverPanel = addPanel((float)mouse.x, maxY - height, 120.0f, height, sf::Color(11, 0,45), sf::Color(31, 24, 96), 2);
	auto noteName = MusicUtilities::getNatural(note.pitch.note);
	Accidental acc = MusicUtilities::getAccidental(note.pitch.note);

	std::string noteRest = "Note";

	std::string pitchString = MusicUtilities::getNoteString(note.pitch.note);
	std::string valueString = MusicUtilities::getValueString(note.value);

	if (note.pitch.note == NoteRest)
	{
		noteRest = "Rest";
		hoverPanel->addText(valueString + noteRest, 16, sf::Color(190,188,216));
	}
	else
	{
		hoverPanel->addText(valueString + noteRest, 16, sf::Color(190, 188, 216));
		hoverPanel->addText("Pitch: " + pitchString, 14, sf::Color(212, 209,255));
		hoverPanel->addText("Interval: " + MusicUtilities::getIntervals(note.pitch.note, key), 14, sf::Color(212, 209, 255));
	}
	unhover = true;
}

void SheetMusicNote::unhoverAction()
{
	runHoverAction = true;
	auto it = std::find(objects.begin(), objects.end(), hoverPanel);
	if (it != objects.end()) {
		objects.erase(it);
	}
}

void SheetMusicNote::setAccidentalWidth(float accidentalWidth)
{
	float oldAccidentalWidth = this->accidentalWidth;
	this->accidentalWidth = accidentalWidth;

	float x = sprite.getPosition().x - oldAccidentalWidth + accidentalWidth;

	sprite.setPosition(sf::Vector2f(x, sprite.getPosition().y));

	for (auto& rectangle : extraLines)
	{
		rectangle.move(sf::Vector2f(accidentalWidth - oldAccidentalWidth, 0));
	}
}


std::pair<sf::Vector2f, sf::Vector2f> SheetMusicNote::getHoverArea()
{
	if (!GUIUtilities::positionInBounds(sf::Mouse::getPosition(*window), getNoteBounds()) && noteCount > 1)
	{
		return getNoteBounds();
	}

	sf::Vector2f position(
		sprite.getPosition().x - sprite.getOrigin().x * sprite.getScale().x,
		sprite.getPosition().y - sprite.getOrigin().y * sprite.getScale().y);

	sf::Vector2f size(
		sprite.getLocalBounds().width * sprite.getScale().x,
		sprite.getLocalBounds().height * sprite.getScale().y);

	return std::pair<sf::Vector2f, sf::Vector2f>(position, size);
}

std::pair<sf::Vector2f, sf::Vector2f> SheetMusicNote::getNoteBounds()
{
	return {
		{sprite.getPosition().x - sprite.getOrigin().x * sprite.getScale().x
		,sprite.getPosition().y - sprite.getOrigin().y * sprite.getScale().x
		+ sprite.getLocalBounds().height * sprite.getScale().y * .75f},

		{sprite.getLocalBounds().width * sprite.getScale().x, 
		sprite.getLocalBounds().height* sprite.getScale().y * .25f}
	};
}

void SheetMusicNote::setWindow(sf::RenderWindow* window)
{
	this->window = window;
	SMAccidental.setWindow(window);
}

RenderObject& SheetMusicNote::getHoverObject()
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
	auto hoverArea = this->getHoverArea();
	if (GUIUtilities::positionInBounds(mousePosition, this->getHoverArea()))
	{
		return *this;
	}

	return *GUIUtilities::getEmptyRenderObject();
}


void SheetMusicNote::loadNote()
{
	halfStep = this->staffHeight / 8.0f;
	float distance;
	float noteScale = 1.0f;

	switch (clef)
	{
	case TrebleClef:
		this->C4Position = 1.25;
		break;
	case BassClef:
		this->C4Position = -0.25;
		break;
	}

	//Notes will be positioned in relation to the middle bar
	//Whole : Position is 0.0, so the top of the rest will be on the bar
	//Half	: Position is 1, so the bottom of the rest is on the bar

	//	"resources/images/sheet_music/rests/quarter.png"
	//	"resources/images/sheet_music/rests/eighth.png"
	//	"resources/images/sheet_music/rests/sixteenth.png"
	//	"resources/images/sheet_music/rests/thirtysecond.png"


	if (note.pitch.note == NoteRest)
	{
		switch (note.value)
		{
		case Whole:
			texture = ResourceManager::getTexture("resources/images/sheet_music/rests/wholehalf.png");
			if (texture == nullptr) {
				// Handle error
				std::cout << "Note Texture unable to load\n";
				return;
			}
			this->C4Position = 0.5f;
			notePointVertical = 0.0;
			notePointHorizontal = 0.5;
			noteScale = 0.125f;
			break;
		case Half:
			texture = ResourceManager::getTexture("resources/images/sheet_music/rests/wholehalf.png");
			if (texture == nullptr) {
				// Handle error
				std::cout << "Note Texture unable to load\n";
				return;
			}
			this->C4Position = 0.5f;
			notePointVertical = 1.1f;
			notePointHorizontal = 0.5;
			noteScale = 0.125f;
			break;
		case Quarter:
			texture = ResourceManager::getTexture("resources/images/sheet_music/rests/quarter.png");
			if (texture == nullptr) {
				// Handle error
				std::cout << "Note Texture unable to load\n";
				return;
			}
			noteScale = 0.625f;
			this->C4Position = 0.5f;
			notePointVertical = 0.5;
			notePointHorizontal = 0.5;
			break;
		case Eighth:
			texture = ResourceManager::getTexture("resources/images/sheet_music/rests/eighth.png");
			if (texture == nullptr) {
				// Handle error
				std::cout << "Note Texture unable to load\n";
				return;
			}
			noteScale = 0.5f;
			this->C4Position = 0.375f;
			notePointVertical = 42.0f / 256.0f;
			notePointHorizontal = 0.25;
			break;
		case Sixteenth:
			texture = ResourceManager::getTexture("resources/images/sheet_music/rests/sixteenth.png");
			if (texture == nullptr) {
				// Handle error
				std::cout << "Note Texture unable to load\n";
				return;
			}
			noteScale = 0.75f;
			this->C4Position = 0.375f;
			notePointVertical = 30.0f / 256.0f;
			notePointHorizontal = 0.25;

			break;
		case ThirtySecond:
			texture = ResourceManager::getTexture("resources/images/sheet_music/rests/thirtysecond.png");
			if (texture == nullptr) {
				// Handle error
				std::cout << "Note Texture unable to load\n";
				return;
			}
			noteScale = 1.0f;
			this->C4Position = 0.125f;
			notePointVertical = 20.0f / 256.0f;
			notePointHorizontal = 0.25;
			break;
		}

		this->sprite.setTexture(*texture);

		float newScale = this->staffHeight * noteScale / sprite.getLocalBounds().height;
		this->sprite.setScale(newScale, newScale);
		this->sprite.setOrigin(sf::Vector2f(sprite.getLocalBounds().width * notePointHorizontal, sprite.getLocalBounds().height * notePointVertical)); //sets the origin to the "center" of the symbol, the part which rests on a note

		sprite.setColor(noteColor);


		distance = 0;
		distance *= halfStep;

		this->positionY = staffY + (C4Position * staffHeight) - distance;


		this->sprite.setPosition(sf::Vector2f(staffX + accidentalWidth + sprite.getOrigin().x * sprite.getScale().x, staffY + (C4Position * staffHeight) - distance));

		return;

	}

	distance = (float)MusicUtilities::getNotesFromMiddleC(note.pitch, true);
	float staffPosition = C4Position - distance / 8.0f;


	if (accidental != Natural)
	{
		SMAccidental.setPosition(staffX, staffPosition);
		accidentalWidth = SMAccidental.getWidth();
	}




	switch (note.value)
	{
	case Whole:
		texture = ResourceManager::getTexture("resources/images/sheet_music/notes/Whole.png");
		if (texture == nullptr) {
			// Handle error
			std::cout << "Note Texture unable to load\n";
			return;
		}
		notePointVertical = 0.5f;
		notePointHorizontal = 0.5f;
		noteScale = 0.25f;
		break;
	case Half:
		texture = ResourceManager::getTexture("resources/images/sheet_music/notes/HalfLow.png");
		if (texture == nullptr) {
			// Handle error
			std::cout << "Note Texture unable to load\n";
			return;
		}
		notePointVertical = 0.86f;
		notePointHorizontal = 0.5f;
		break;
	case Quarter:
		texture = ResourceManager::getTexture("resources/images/sheet_music/notes/QuarterLow.png");
		if (texture == nullptr) {
			// Handle error
			std::cout << "Note Texture unable to load\n";
			return;
		}
		notePointVertical = 0.875f;
		notePointHorizontal = 0.5f;
		break;
	case Eighth:
		texture = ResourceManager::getTexture("resources/images/sheet_music/notes/EighthSingleLow.png");
		if (texture == nullptr) {
			// Handle error
			std::cout << "Note Texture unable to load\n";
			return;
		}
		notePointVertical = 0.875f;
		notePointHorizontal = 0.25f;
		break;
	case Sixteenth:
		texture = ResourceManager::getTexture("resources/images/sheet_music/notes/SixteenthSingleLow.png");
		if (texture == nullptr) {
			// Handle error
			std::cout << "Note Texture unable to load\n";
			return;
		}
		notePointVertical = 0.875f;
		notePointHorizontal = 0.25f;
		break;
	case ThirtySecond:
		texture = ResourceManager::getTexture("resources/images/sheet_music/notes/Whole.png");
		if (texture == nullptr) {
			// Handle error
			std::cout << "Note Texture unable to load\n";
			return;
		}
		notePointVertical = 0.875f;
		notePointHorizontal = 0.25f;
		break;
	}

	this->sprite.setTexture(*texture);

	float newScale = this->staffHeight * noteScale / sprite.getLocalBounds().height;
	this->sprite.setScale(newScale, newScale);
	this->sprite.setOrigin(sf::Vector2f(sprite.getLocalBounds().width * notePointHorizontal, sprite.getLocalBounds().height * notePointVertical)); //sets the origin to the "center" of the symbol, the part which rests on a note
	this->sprite.setScale(newScale, newScale);

	sprite.setColor(noteColor);

	

	distance *= halfStep;

	this->positionY = staffY + (C4Position * staffHeight) - distance;


	this->sprite.setPosition(sf::Vector2f(staffX + accidentalWidth + sprite.getOrigin().x * sprite.getScale().x, staffY + (C4Position * staffHeight) - distance));

	distance = (float)MusicUtilities::getNotesFromMiddleC(note.pitch, true);

	switch (clef)
	{
	case TrebleClef:
		if (!(distance > 0 && distance < 12))
		{
			addLinesToNote(note.pitch);
		}
		break;
	case BassClef:

		if (!(distance < 0 && distance > -12))
		{
			addLinesToNote(note.pitch);
		}
		break;
	}



}