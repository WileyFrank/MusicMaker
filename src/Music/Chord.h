#pragma once
#include <vector>
#include "MusicUtilities.h"


class Chord
{
private:
	ChordType type;
	Pitch rootNote;
	std::vector<Pitch> notes;

public:
	Chord(Pitch root, ChordType type);
	Chord(PitchEnum root, int octave, ChordType type);
};

