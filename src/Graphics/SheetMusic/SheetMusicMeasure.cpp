#include "SheetMusicMeasure.h"
#include <cmath>

void SheetMusicMeasure::checkMeasure()
{
}

void SheetMusicMeasure::loadSheetNotes()
{
	SheetMusicNote* newNote;
	float width = 0.0f;
	float position = x;

	float accidentalOffset;

	for (auto& note : notes)
	{
		accidentalOffset = 0;
		width = 0.0f;
		auto accidental = Natural;

		for (auto& singleNote : note.second)
		{
			//get accidentals

			if (singleNote.pitch.note != NoteRest)
			{
				accidental = MusicUtilities::getAccidentalsInKey(singleNote, keySignature);
			}


			newNote = new SheetMusicNote(position, y, staffHeight, clef, singleNote, accidental, this->keySignature);
			newNote->setWindow(window);
			newNote->setNoteCount((int)note.second.size());
			sheetNotes[note.first].push_back(newNote);

			if (width < newNote->getWidth())
			{
				width = newNote->getWidth();
			}

			if (accidentalOffset < newNote->getAccidentalWidth())
			{
				accidentalOffset = newNote->getAccidentalWidth();
			}
		}

		if (accidentalOffset > 0)
		{
			for (auto& singleNote : sheetNotes[note.first])
			{
				singleNote->setAccidentalWidth(accidentalOffset);
			}
		}
		position += width + noteGap;
	}
}

Note SheetMusicMeasure::getRest(float beat, bool dotted)
{
	NoteValue value;
	for (int i = 0; i < 6; i++)
	{
		float interval = 4.0f / (int)(pow(2, i));
		if (std::fmod(beat, interval) == 0.0f)
		{
			value = (NoteValue)(int)(4.0f * quarterValue / interval);
			auto longest = getLongestRest(beat, dotted).value;
			if (value >= longest)
			{
				return Note({ { NoteRest, 0 },  value});
			}
		}
	}

	std::cerr << "Error generating rests\n";
	return Note();
}

Note SheetMusicMeasure::getLongestRest(float beat, bool dotted)
{
	//At any given place in the music, there can be
	//n notes starting
	//n notes playing

	//If there is a note starting in the next beat, or any note is currently playing, there is no rest
	//Otherwise, is no notes are playing, get the distance until the next note to be played, this should be recursively rested


	//If any note is playing return no note (An empty rest)
	for (auto& noteVec : notes) {
		for (auto& note : noteVec.second)
		{
			if (noteVec.first < beat && noteVec.first + (1.0f / ((int)note.value * quarterValue)) > beat)
			{
				return Note({ Pitch({NoteRest,-12}), NoRest });
			}
		}
	}

	//find the next note

	std::pair<float, std::vector<Note>> nextBeat;

	float next = (float)timeSignature.denominator;

	for (auto& pair : notes) {
		if (pair.first > beat && pair.first < next)
		{
			next = pair.first;
			break;
		}

	}

	
	//restGap is the float value of the beats between the parameter beat, and the next played note.
	//This is in terms of the time signature, so in 2/2 1.0 is a half note
	//This can be used with the quarter value in 2/2 it would be 0.5 so divide the gap by it to get amount of quarterbeats
	float restGap = next - beat;
	float quarterRestGap = restGap / quarterValue;


	return Note({ Pitch({NoteRest,-12 }), MusicUtilities::getLargestNote(quarterRestGap)
});
}

SheetMusicMeasure::SheetMusicMeasure()
	:x(0), y(0), width(0), staffHeight(0), timeSignature(TimeSignature({ 4, 4 })), quarterValue(1.0f), noteCount(0), noteGap(10), clef(TrebleClef) 
{
}

SheetMusicMeasure::SheetMusicMeasure(float x, float y, float staffHeight, Clef clef, TimeSignature timeSignature, KeySignature keySignature)
	:x(x), y(y), width(0), staffHeight(staffHeight), clef(clef), 
	timeSignature(timeSignature), keySignature(keySignature), 
	quarterValue(MusicUtilities::getBeats(timeSignature, Quarter)), noteCount(0), noteGap(staffHeight/5.0f)
{
}

SheetMusicMeasure::~SheetMusicMeasure()
{
}

void SheetMusicMeasure::setupStaff(float x, float y, float staffHeight, Clef clef)
{
	this->clef = clef;
	this->x = x;
	this->y = y;
	this->staffHeight = staffHeight;
	loadSheetNotes();
}

float SheetMusicMeasure::addNote(Note note, float beat)
{

	notes[beat].push_back(note);
	return 	beat + MusicUtilities::getBeats(timeSignature, note.value);
}

std::vector<Note> SheetMusicMeasure::getNotes(float beat)
{
	//At any given place in the music, there can be
	//n notes starting
	//n notes playing

	std::vector<Note> currentNotes;

	if (notes[beat].size() > 0)
	{
		return notes[beat];
	}
	else
	{
		currentNotes.push_back(getRest(beat));
	}


	return currentNotes;
}

std::vector<Note> SheetMusicMeasure::getPlaying(float beat)
{
	//At any given place in the music, there can be
	//n notes starting
	//n notes playing

	std::vector<Note> currentNotes;

	
	for (auto& note : notes)
	{
		for (auto& singleNote : note.second)
		{
			if (note.first <= beat && note.first + MusicUtilities::getBeats(timeSignature, singleNote.value) > beat)
			{
				currentNotes.push_back(singleNote);
			}
		}
	}

	if (currentNotes.size() == 0)
	{
		currentNotes.push_back(getRest(beat));
	}


	return currentNotes;
}

void SheetMusicMeasure::addRests()
{
	float currentBeat = 1.0f;
	float maxTime = 0.0f;
	float maxBeats = 0.0f;


	while (currentBeat < (float)timeSignature.denominator)
	{
		auto currentNotes = getPlaying(currentBeat);

		//if the note is a rest
		if (currentNotes[0].pitch.note == NoteRest)
		{
			notes[currentBeat] = currentNotes;
		}

		currentNotes = getNotes(currentBeat);

		for (auto& pair : currentNotes) {
			maxBeats = 1.0f / (float)pair.value * quarterValue * 4.0f;
			if (maxBeats > maxTime)
			{
				maxTime = maxBeats;
			}
		}

		currentBeat += maxBeats;
	}
}

void SheetMusicMeasure::draw()
{
	SheetMusicNote* active = __nullptr;

	for (auto& pair : sheetNotes) {
		for (SheetMusicNote* note : pair.second)
		{
			if (note->getHover())
			{
				active = note;
				active->draw();
				continue;
			}
			note->draw();
		}
	}

	if (active != nullptr)
	{
		active->draw();
	}

	for (auto& element : objects)
	{
		element->draw();
	}

}

std::pair<sf::Vector2f, sf::Vector2f> SheetMusicMeasure::getHoverArea()
{
	sf::Vector2f position(x, y);
	sf::Vector2f size(getWidth(), staffHeight);

	return std::pair<sf::Vector2f, sf::Vector2f>(position, size);
}

RenderObject& SheetMusicMeasure::getHoverObject()
{
	RenderObject* object = GUIUtilities::getEmptyRenderObject();


	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
	for (auto& pair : sheetNotes)
	{
		for (SheetMusicNote* note : pair.second)
		{
			note->setOverrideAction(true);
			note->setHover(false);
			if (note->getHoverObject().getType() != EmptyRenderObject)
			{
				this->hoverBeat = -1.0f;
				
				note->setOverrideAction(false);
				return note->getHoverObject();
			}
		}
	}

	if (GUIUtilities::positionInBounds(mousePosition, getHoverArea()))
	{

		float noteX = x;

		for (auto& sheetNoteMap : sheetNotes)
		{

			float max = 0.0f;
			for (auto& note : sheetNoteMap.second)
			{
				if (max < note->getHoverArea().second.x)
				{
					max = note->getHoverArea().second.x;
				}
			}

			noteX += max;
			noteX += noteGap;

			if (noteX > mousePosition.x)
			{
				if (this->hoverBeat != sheetNoteMap.first)
				{
					this->setHoverStart(std::chrono::high_resolution_clock::now());
					for (auto& note : sheetNotes[sheetNoteMap.first])
					{
						note->update();
					}
					unhoverUpdate();
				}
				this->hoverBeat = sheetNoteMap.first;
				
				if (sheetNotes[hoverBeat].size() > 1)
				{
					for (auto& note : sheetNotes[hoverBeat])
					{
						//Used so the note hover menu timer doesn't start from a measure, only from the note itself
						note->setHoverStart(std::chrono::high_resolution_clock::now());
					}
				}
				else
				{
					sheetNotes[hoverBeat][0]->setOverrideAction(false);
				}


				return *this;
			}
		}
		return *object;
	}

	//Notes take priority over the measure itself
	hoverBeat = -1;
	return *object;
}

void SheetMusicMeasure::setHover(bool hover)
{
	this->hover = hover;
	if (!hover)
	{
		for (auto& pair : sheetNotes) {
			for (SheetMusicNote* note : pair.second)
			{
				note->setHover(hover);
			}
		}
	}
}

void SheetMusicMeasure::setWindow(sf::RenderWindow* window)
{
	this->window = window;
	for (auto& pair : sheetNotes) {
		for (auto* note : pair.second)
		{
			note->setWindow(window);
		}
	}
}

void SheetMusicMeasure::updateColors()
{
	for (auto& pair : sheetNotes) {
		for (auto* note : pair.second)
		{
			note->setStaffColor(staffColor);
			note->setNoteColor(noteColor);
		}
	}
}

void SheetMusicMeasure::reload()
{
	sheetNotes.clear();
	loadSheetNotes();
	updateColors();
}

void SheetMusicMeasure::update()
{

	if (this->hoverBeat >= 0)
	{
		if (sheetNotes[hoverBeat].size() == 1)
		{
			sheetNotes[hoverBeat][0]->setHover(true);
		}

		for (auto& note : sheetNotes[hoverBeat])
		{
			note->setHover(true);
		}
	}

	for (auto& pair : sheetNotes) {
		for (SheetMusicNote* note : pair.second)
		{
			note->update();
		}
	}

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

void SheetMusicMeasure::hoverUpdate()
{
	if (!unhover)
	{
		this->hoverStart = std::chrono::high_resolution_clock::now();
		//Initial hover actions
	}

	auto timeNow = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(timeNow - hoverStart);
	if (duration.count() >= 300 && runHoverAction)
	{
		hoverAction();
		runHoverAction = false;
	}
}

void SheetMusicMeasure::unhoverUpdate()
{
	unhoverAction();
}

void SheetMusicMeasure::hoverAction()
{
	if (sheetNotes[hoverBeat].size() <= 1)
	{
		return;
	}
	auto mouse = sf::Mouse::getPosition(*window);
	float minY = -1;
	
	for (auto& note : sheetNotes[hoverBeat])
	{
		auto noteBounds = note->getHoverArea();

		if (noteBounds.first.y < minY || minY == -1) minY = noteBounds.first.y;
	}
	


	float height = 60.0f;

	hoverPanel = addPanel((float)mouse.x, minY - height, 120.0f, height, sf::Color(11, 0, 45), sf::Color(31, 24, 96), 2);

	std::string notesString = "";
	std::vector<Pitch> pitches;

	for (auto& note : notes[hoverBeat])
	{
		std::string pitchString = MusicUtilities::getNoteString(note.pitch.note);

		if (note.pitch.note != NoteRest)
		{
			notesString += pitchString + ", ";
		}
		pitches.push_back(note.pitch);
	}
	
	auto chord = MusicUtilities::findChord(pitches);

	if (chord.getType() != UNRECOGNIZEDCHORD)
	{
		std::string chordString = "Chord: " + MusicUtilities::getNoteString(chord.getRoot().note) + " " + MusicUtilities::getChordString(chord.getType());
		hoverPanel->addText(chordString, 16, sf::Color(190, 188, 216));
	}
	
	if (notesString.length() > 0)
	{
		notesString = "Notes: " + notesString.substr(0, notesString.length() - 2);
		hoverPanel->addText(notesString, 16, sf::Color(190, 188, 216));
	}
}

void SheetMusicMeasure::unhoverAction()
{
	runHoverAction = true;
	auto it = std::find(objects.begin(), objects.end(), hoverPanel);
	if (it != objects.end()) {
		objects.erase(it);
	}
}

float SheetMusicMeasure::getWidth()
{
	float width = 0;
	float max = 0;

	for (auto& sheetNoteMap : sheetNotes)
	{
		max = 0;
		for (auto& note : sheetNoteMap.second)
		{
			if (max < note->getWidth())
			{
				max = note->getWidth();
			}
		}

		width += max + noteGap;
	}
	
	return width;
}

void SheetMusicMeasure::setStaffColor(sf::Color color)
{

	staffColor = color;
	for (auto& sheetNoteMap : sheetNotes)
	{
		for (auto& note : sheetNoteMap.second)
		{
			note->setStaffColor(staffColor);
		}
	}
}

void SheetMusicMeasure::setNoteColor(sf::Color color)
{
	noteColor = color;
	staffColor = color;
	for (auto& sheetNoteMap : sheetNotes)
	{
		for (auto& note : sheetNoteMap.second)
		{
			note->setNoteColor(color);
		}
	}
}

void SheetMusicMeasure::setHoverColor(sf::Color color)
{
	for (auto& sheetNoteMap : sheetNotes)
	{
		for (auto& note : sheetNoteMap.second)
		{
			note->setHoverColor(color);
		}
	}
}
