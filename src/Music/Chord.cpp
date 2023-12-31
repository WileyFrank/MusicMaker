#include "Chord.h"

Chord::Chord(Pitch root, ChordType type)
{
	this->type = type;
	this->rootNote = root;
}

Chord::Chord(PitchEnum root, int octave, ChordType type)
{
	this->type = type;
	this->rootNote.note = root;
	this->rootNote.octave = octave;

	notes = MusicUtilities::getNotes(rootNote, type);


}
