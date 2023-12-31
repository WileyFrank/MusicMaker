#include "Chord.h"

Chord::Chord(Pitch root, ChordType type)
{
	this->type = type;
	this->rootNote = root;
	this->modifiers = {};
}

Chord::Chord(Pitch root, ChordType type, std::vector<int> mods)
	:rootNote(root), type(type), modifiers(mods)
{
	notes = MusicUtilities::getNotes(rootNote, type);
}

Chord::Chord(PitchEnum root, int octave, ChordType type)
{
	this->type = type;
	this->rootNote.note = root;
	this->rootNote.octave = octave;
	this->modifiers = {};

	notes = MusicUtilities::getNotes(rootNote, type);


}
