#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <mutex>

enum ChordType {
	MAJOR = 0,
	MINOR,
	DIMINISHED,
	AUGMENTED,
	MAJOR7,
	DOMINANT7,
	MINOR7,
	HALFDIMINISHED7,
	DIMINISHED7,
	NINTH,
	ELEVENTH,
	THIRTEENTH,
	SUS2,
	SUS4,
	ADD9,
	ADD11,
	ADD13,
	UNRECOGNIZEDCHORD
};

//if the pitch is 1 - 12 it is the actual pitch, adding or subtracting 12 is used to store sharp and flat
enum PitchEnum {
	NoteRest = -12,
	NoteC = 0,
	NoteCD = 1,
	NoteCs = 13,
	NoteDf = -11,
	NoteD = 2,
	NoteDE = 3,
	NoteDs = 15,
	NoteEf = -9,
	NoteE = 4,
	NoteEs = 17,
	NoteFf = -8,
	NoteF = 5,
	NoteFG = 6,
	NoteFs = 18,
	NoteGf = -6,
	NoteG = 7,
	NoteGA = 8,
	NoteGs = 20,
	NoteAf = -4,
	NoteA = 9,
	NoteAB = 10,
	NoteAs = 22,
	NoteBf = -2,
	NoteB = 11,
	NoteBs = 12,
	NoteCf = -1
};

enum NoteValue
{
	NoRest = 0,
	Whole = 1,
	Half = 2,
	Quarter = 4,
	Eighth = 8,
	Sixteenth = 16,
	ThirtySecond = 32,
	EighthTriplet = 12,
	SixteenthTriplet = 24,
	ThirtySecondTriplet	= 48
};

enum Accidental
{

	DoubleFlat = -2,
	Flat = -1,
	Natural = 0,
	Sharp = 1,
	DoubleSharp = 2,
	ForcedNatural = 3
};

struct Pitch {
	PitchEnum note;
	int octave;
};

struct Note {
	Pitch pitch = { (PitchEnum)0,0 };
	NoteValue value = (NoteValue)0;
	bool isDotted = false, isTied = false;
};


struct TimeSignature {
	int numerator = 4;
	int denominator = 4;
};


//Each of the notes can be marked as sharp flat or natural
struct KeySignature {
	std::string name = "C Major";
	std::map<PitchEnum, Accidental> key = { 
		std::make_pair(NoteF, Natural),
		std::make_pair(NoteC, Natural),
		std::make_pair(NoteG, Natural),
		std::make_pair(NoteD, Natural),
		std::make_pair(NoteA, Natural),
		std::make_pair(NoteE, Natural),
		std::make_pair(NoteB, Natural),
	};
};

enum Clef { TrebleClef = 0, BassClef };

class MusicUtilities
{
public:

	// |-|-|-|-|-|-|-|-|-|-| SCALES AND CHORDS |-|-|-|-|-|-|-|-|-|-|-|-|

	static KeySignature getKey(PitchEnum root, ChordType type)
	{
		KeySignature key;
		key.name = getNoteString(root) + " " + getChordString(type);
		PitchEnum tempPitch = (PitchEnum)-12;
		std::vector<int> notePattern;
		int noteCount = 0;

		//The natural note name
		PitchEnum currentNote;

		//the raw pitch
		PitchEnum currentPitch;
		Accidental currentAccidental;

		switch (type)
		{
		case MAJOR:
			//R-II-III-IV-V-VI-VII-R+
			//0-2 - 4 -5 -7-9 - 11

			noteCount = 7;
			notePattern = { 0, 2, 4, 5, 7, 9, 11 };

			currentPitch = (PitchEnum)((root) % 12);
			currentNote = getNatural(root);

			for (int i : notePattern)
			{
				currentPitch = (PitchEnum)((root + i) % 12);
				currentAccidental = getAccidental(currentPitch, currentNote);
				key.key[currentNote] = currentAccidental;
				currentNote = getNextNatural(currentNote);
			} 
			return key;
		default:
			return key;
		}
	}

	static std::vector<Pitch> getNotes(Pitch root, ChordType type)
	{
		std::vector<Pitch> notes;
		notes.push_back(root);
		auto rootNote = root.note;
		auto rootOctave = root.octave;
		int newOctave;
		int newNoteNum;
		int noteCount;
		std::vector<int> semitonePattern;




		switch (type)
		{
		case MAJOR: //I III V
			noteCount = 3;
			semitonePattern = { 4, 7 };

			for (int i = 0; i < noteCount - 1; i++)
			{
				newOctave = rootOctave;

				if (rootNote + semitonePattern[i] > 12)
				{
					newOctave += (int)((rootNote + semitonePattern[i]) / 12);
				}

				newNoteNum = ((((int)root.note - 1 + semitonePattern[i]) % 12) + 1);
				
				Pitch newNote1 = { (PitchEnum)newNoteNum, newOctave };
				notes.push_back(newNote1);
			}

			return notes;
		case MINOR: //I iii(IIIb) V
			noteCount = 3;
			semitonePattern = { 3, 7 };

			for (int i = 0; i < noteCount - 1; i++)
			{
				newOctave = rootOctave;

				if (rootNote + semitonePattern[i] > 12)
				{
					newOctave += (int)((rootNote + semitonePattern[i]) / 12);
				}

				newNoteNum = ((((int)root.note - 1 + semitonePattern[i]) % 12) + 1);

				Pitch newNote1 = { (PitchEnum)newNoteNum, newOctave };
				notes.push_back(newNote1);
			}

			return notes;
		case DIMINISHED:
			break;
		case AUGMENTED:
			break;
		case MAJOR7:
			break;
		case DOMINANT7:
			break;
		case MINOR7:
			break;
		case HALFDIMINISHED7:
			break;
		case DIMINISHED7:
			break;
		case NINTH:
			break;
		case ELEVENTH:
			break;
		case THIRTEENTH:
			break;
		case SUS2:
			break;
		case SUS4:
			break;
		case ADD9:
			break;
		case ADD11:
			break;
		case ADD13:
			break;
		default:
			std::cerr << "Uninitialized Chord Type\n";
		}
		return notes;
	}
	
	static ChordType findChord(std::vector<Pitch> pitches)
	{
		//at this point a chord can only be 3+ notes
		if (pitches.size() <= 2)
		{
			return UNRECOGNIZEDCHORD;
		}

		auto intervals = generateIntervals(pitches);
		return UNRECOGNIZEDCHORD;
	}

	static std::vector<int> getNotePattern(ChordType type)
	{
		switch (type)
		{
		case MAJOR:
			return { 0, 2, 4, 5, 7, 9, 11 };
		case MINOR: //This may be wrong TODO
			return { 0, 2, 3, 5, 6, 8, 11 };
		}

		return {};
	}

	static std::vector<int> generateIntervals(std::vector<Pitch> pitches)
	{
		std::vector<int> intervals = { };
		auto start = pitches.front();

		for (int i = 1; i < pitches.size(); i++)
		{
			int newInterval = negativeMod((negativeMod(pitches[i].note, 12) - negativeMod(start.note, 12)), 12);
			start = pitches[i];
			intervals.push_back(newInterval);
		}
		return intervals;
	}

	static std::vector<int> generateRootIntervals(std::vector<Pitch> pitches)
	{

		std::vector<int> intervals = { 0 };
		auto start = pitches.front();

		for (int i = 1; i < pitches.size(); i++)
		{
			int newInterval = negativeMod((negativeMod(pitches[i].note, 12) - negativeMod(start.note, 12)), 12);
			intervals.push_back(newInterval);
		}
		return intervals;
	}

	static std::vector<Pitch> orderPitchAscending(std::vector<Pitch> pitches)
	{
		//Sorts the pitched in ascending order;

		if (pitches.size() <= 1)
		{
			return pitches;
		}

		for (int i = 1; i < pitches.size(); i++)
		{
			int j = i - 1;

			while (j >= 0 && getSemitoneDistance(pitches[j + 1], pitches[j]) > 0)
			{
				auto temp = pitches[j + 1];
				pitches[j + 1] = pitches[j];
				pitches[j] = temp;
				j--;
			}
		}

		return pitches;

	}

	static float getBeats(TimeSignature timeSignature, NoteValue value)
	{
		return ((float)timeSignature.denominator / (float)(int)value);
	}

	// |-|-|-|-|-|-|-|-|-|-| PITCH AND ACCIDENTALS |-|-|-|-|-|-|-|-|-|-|-|-|

	static Pitch getPitch(PitchEnum pitch, Accidental accidental, int octave)
	{
		/*switch (accidental)
		{
		case Natural:
			return Pitch{ pitch, octave };
		case Flat:
		case DoubleFlat:
		}*/
	}

	static PitchEnum getNatural(PitchEnum pitch)
	{
		Accidental acc = getAccidental(pitch);

		if (acc == Natural)
			return pitch;

		return (PitchEnum)negativeMod(((int)pitch - acc), 12);
	}

	//Param pitch should be natural, the purpose is the get the next note up
	//C -> D
	//E -> F
	static PitchEnum getNextNatural(PitchEnum pitch)
	{
		int tempPitch = (pitch + 1) % 12;
		while (getAccidental((PitchEnum)tempPitch) != Natural && tempPitch != pitch)
		{
			tempPitch = (tempPitch + 1) % 12;
		}
		return (PitchEnum)tempPitch;
	}

	static Accidental getAccidental(PitchEnum pitch)
	{
		if (pitch == NoteC ||
			pitch == NoteD ||
			pitch == NoteE ||
			pitch == NoteF ||
			pitch == NoteG ||
			pitch == NoteA ||
			pitch == NoteB)
		{
			return Natural;
		}
		else
		{
			if ((int)pitch > -1)
			{
				return Sharp;
			}
			return Flat;
		}
	}

	//Name determines what name the note will be stored under
	//Pitch =	Eb	F	F
	//Name =	D	E	F	NAME SHOULD ALWAYS BE NATURAL it is used to index the KeySignature map
	//Note =	D#	E#	F	etc.
	static Accidental getAccidental(PitchEnum pitch, PitchEnum name)
	{
		if (getAccidental(name) != Natural)
		{
			std::cerr << "Name pitch should be natural\n";
			return Natural; // THIS IS NOT TO BE USED
		}

		if (pitch == name || pitch % 12 == name)
		{
			return Natural;
		}

		int distance = getSemitoneDistance(name, pitch);

		if (abs(distance) <= 2)
		{
			return (Accidental)distance;
		}

		std::cerr << "Pitch is too far to name (3+)\n";
		return Natural; // THIS IS NOT TO BE USED
	}

	static Accidental getAccidentalCircleOfFifths(PitchEnum pitch)
	{
		if ((int)pitch < 0)
			return (Flat);

		if ((int)pitch > 12)
			return Sharp;

		return Natural;
	}

	static Accidental getAccidentalsInKey(Note note, KeySignature key)
	{
		auto natural = getNatural(note.pitch.note);

		int accidentalInt = (getAccidental(note.pitch.note, natural) - key.key[natural]);
		
		if (getAccidental(note.pitch.note) == Natural && accidentalInt != 0)
			return ForcedNatural;

		if (accidentalInt > -3 && accidentalInt < 3)
		{
			return (Accidental)accidentalInt;
		}

		std::cerr << "Note requires more than 2 accidentals\n";
		return Natural;

	}

	static NoteValue getLargestNote(float quarterBeats)
	{
		NoteValue value;
		for (int i = 0; i < 6; i++)
		{
			value = (NoteValue)(int)pow(2, i);

			if (1.0f / quarterBeats * 4 <= (int)value)
			{
				return value;
			}

		}

		return (NoteValue)0;
	}

	// |-|-|-|-|-|-|-|-|-|-| NOTE DISTANCE AND INTERVALS |-|-|-|-|-|-|-|-|-|-|-|-|

	static int getSemitoneDistance(Pitch pitchA, Pitch pitchB)
	{
		int octaves = pitchB.octave - pitchA.octave;
		return (negativeMod(pitchB.note, 12) - negativeMod(pitchA.note, 12)) + (octaves * 12);

	}

	static int getSemitoneDistance(PitchEnum pitchA, PitchEnum pitchB)
	{
		int distance = pitchB - pitchA;

		while ((abs(distance) > 6))
		{
			if (distance > 6)
				distance -= 12;
			if (distance < -6)
				distance += 12;
		}
		return (distance);

	}

	static int getSemitonesFromMiddleC(Pitch pitch)
	{
		if ((int)pitch.note < 0 || (int)pitch.note > 11)
		{
			pitch.note = (PitchEnum)((int)pitch.note % 12);
		}

		int distanceToC = pitch.note - NoteC;

		return distanceToC + ((pitch.octave - 4) * 12);

	}

	static int getNotesFromMiddleC(Pitch pitch, bool namePrecedence = false)
	{
		if (namePrecedence == true)
		{
			return getNotesFromMiddleC(Pitch({getNatural(pitch.note), pitch.octave}), false);
		}
		int semitones = getSemitonesFromMiddleC(pitch);
		int octaves = 0;
		//TODO make better
		while (semitones < 0)
		{
			semitones += 12;
			octaves -= 1;
		}

		octaves += (int)(semitones / 12);


		semitones = negativeMod(semitones, 12);


		switch (semitones)
		{
		case 0:
			return octaves * 7;
		case 1:
			return octaves * 7;
		case 2:
			return octaves * 7 + 1;
		case 3:
			return octaves * 7 + 1;
		case 4:
			return octaves * 7 + 2;
		case 5:
			return octaves * 7 + 3;
		case 6:
			return octaves * 7 + 3;
		case 7:
			return octaves * 7 + 4;
		case 8:
			return octaves * 7 + 4;
		case 9:
			return octaves * 7 + 5;
		case 10:
			return octaves * 7 + 5;
		case 11:
			return octaves * 7 + 6;
		}
		return 0;
	}

	// |-|-|-|-|-|-|-|-|-|-| DISPLAY STRINGS |-|-|-|-|-|-|-|-|-|-|-|-|

	static std::string getNoteString(PitchEnum pitch)
	{
		auto noteName = MusicUtilities::getNatural(pitch);
		Accidental acc = MusicUtilities::getAccidental(pitch);

		std::map<Accidental, std::string> accidentalMap = {
		{DoubleFlat, "ff"},
		{Flat, "f"},
		{Natural, ""},
		{Sharp, "#"},
		{DoubleSharp, "##"},
		};

		std::map<PitchEnum, std::string> pitchMap = {
			{NoteC, "C"},
			{NoteD, "D"},
			{NoteE, "E"},
			{NoteF, "F"},
			{NoteG, "G"},
			{NoteA, "A"},
			{NoteB, "B"},
		};

		return pitchMap[noteName] + accidentalMap[acc];
	}

	static std::string getChordString(ChordType chord)
	{

		std::map<ChordType, std::string> chordMap = {
		{MAJOR, "Major"},
		{MINOR, "Minor"},
		{MAJOR7, "Major7"},
		{MAJOR7, "Minor7"},
		};

		return chordMap[chord];
	}

	static std::string getValueString(NoteValue value)
	{
		std::map<NoteValue, std::string> valueMap = {
		{Whole, "Whole "},
		{Half, "Half "},
		{Quarter, "Quarter "},
		{Eighth, "Eighth "},
		{Sixteenth, "Sixteenth "},
		{ThirtySecond, "Thirty Second\n"},
		};

		return valueMap[value];
	}

	// |-|-|-|-|-|-|-|-|-|-| GENERAL FUNCTIONALITY |-|-|-|-|-|-|-|-|-|-|-|-|
	static int negativeMod(int dividend, int divisor)
	{
		if (divisor < 0)
			return -1;

		while (dividend < 0)
		{
			dividend += divisor;
		}

		return (dividend % divisor);
	}
};




class Scale
{
private:
	Pitch root;
	ChordType type;
	std::vector<Pitch> notes;
	std::vector<int> accidentals;
	KeySignature key;

	void loadNotes()
	{
		bool empty = notes.empty();
		PitchEnum previousNatural = (PitchEnum)0;
		int octave = root.octave;
		Pitch newPitch;
		PitchEnum currentPitch;

		std::vector<int> notesPattern = MusicUtilities::getNotePattern(type);

		auto currentNatural = MusicUtilities::getNatural(root.note);
		Accidental currentAccidental;

		for (int i : notesPattern) // 0 - 6 see Note Patterns in MusicUtilities
		{
			currentPitch = (PitchEnum)((root.note + i) % 12);
			currentAccidental = MusicUtilities::getAccidental(currentPitch, currentNatural);

			accidentals.push_back(currentAccidental - key.key[currentNatural]);

			//If the note loops from B(11) to C(0) add an octave
			if (previousNatural > currentNatural)
				octave += 1;

			previousNatural = currentNatural;
			newPitch = Pitch({ currentNatural, octave });

			notes.push_back(newPitch);

			currentNatural = MusicUtilities::getNextNatural(currentNatural);
		}
	}

public:
	Scale(Pitch root, ChordType type, KeySignature key)
		:root(root), type(type), key(key)
	{
		loadNotes();
	}

	Scale(Pitch root, ChordType type)
		:root(root), type(type), key(key) //an empty key is CMaj
	{ 
		loadNotes();
	}
};