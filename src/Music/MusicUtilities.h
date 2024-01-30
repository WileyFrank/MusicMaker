#pragma once
#include "SFML/Graphics.hpp"
#include <fmod.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <mutex>

enum Instrument {
	PianoInstrument,
	ToneInstrument
};

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

class Chord
{
private:
	ChordType type;
	Pitch rootNote;
	std::vector<int> modifiers;
	std::vector<Pitch> notes;

public:
	Chord(Pitch root, ChordType type);
	Chord(Pitch root, ChordType type, std::vector<int> mods);
	Chord(PitchEnum root, int octave, ChordType type);

	//getters
	ChordType getType() { return type; }
	Pitch getRoot() { return rootNote; }
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
	PitchEnum root = NoteC;
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
private: 
	static std::map<std::string, FMOD::Sound*> soundCache;
	static std::mutex soundCacheLock;
public:

	// |-|-|-|-|-|-|-|-|-|-| NOTE SOUND FILES |-|-|-|-|-|-|-|-|-|-|-|-|
	static FMOD::Sound* getSound(const std::string& path, FMOD::System* system) {
		// Check if the sound is already loaded
		std::lock_guard<std::mutex> lock(soundCacheLock);

		auto it = soundCache.find(path);
		if (it != soundCache.end()) {
			// Return the cached sound
			return it->second;
		}

		// Sound not in cache, load it
		FMOD::Sound* sound;
		FMOD_RESULT result = system->createSound(path.c_str(), FMOD_3D, nullptr, &sound);
		if (result != FMOD_OK) {
			// Handle error
			std::cout << "Error creating sound (Music Utilities):  " << path << std::endl;
			return nullptr;
		}

		// Store the sound in the cache
		soundCache[path] = sound;
		return sound;
	}

	static void releaseAllCachedSounds() {
		for (auto& entry : soundCache) {
			entry.second->release();
		}
		soundCache.clear();
	}

	// |-|-|-|-|-|-|-|-|-|-| SCALES AND CHORDS |-|-|-|-|-|-|-|-|-|-|-|-|

	static KeySignature getKey(PitchEnum root, ChordType type)
	{
		KeySignature key;
		key.name = getNoteString(root) + " " + getChordString(type);
		key.root = root;
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
		case MINOR:
			//R-II-iii-IV-V-vi-vii-R+
			//0-2 -3 -5 -7-8 -10

			noteCount = 7;
			notePattern = { 0, 2, 3, 5, 7, 8, 10};

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
	
	static float getBeats(TimeSignature timeSignature, NoteValue value)
	{
		return ((float)timeSignature.denominator / (float)(int)value);
	}

	// |-|-|-|-|-|-|-|-|-|-| CHORD DETECTION |-|-|-|-|-|-|-|-|-|-|-|-|
	
	static Chord findChord(std::vector<Pitch> pitches)
	{
		//at this point a chord can only be 3+ notes

		if (pitches.size() <= 2)
		{
			return Chord({NoteC, 0}, UNRECOGNIZEDCHORD);
		}

		auto ascending = orderPitchAscending(pitches);
		auto intervals = generateRootIntervals(ascending);


		auto chord = findChord(intervals);
		if (chord != UNRECOGNIZEDCHORD)
			return Chord(ascending[0], findChordOctave(intervals).first);
		else
		{
			auto chordOctave = findChordOctave(intervals);
			return Chord(ascending[0], chordOctave.first, chordOctave.second);
		}

		return Chord({ NoteC, 0 }, UNRECOGNIZEDCHORD);

	}

	static ChordType findChord(std::vector<int> intervals)
	{
		for (int type = MAJOR; type < UNRECOGNIZEDCHORD; type++)
		{
			std::vector<int> semitonePattern = {};
			switch (type) {
			case MAJOR: // I III V
				semitonePattern = { 0, 4, 7 };
				break;
			case MINOR: // I iii V
				semitonePattern = { 0, 3, 7 };
				break;
			case DIMINISHED: // I iii bV
				semitonePattern = { 0, 3, 6 };
				break;
			case AUGMENTED: // I III #V
				semitonePattern = { 0, 4, 8 };
				break;
			case MAJOR7: // I III V VII
				semitonePattern = { 0, 4, 7, 11 };
				break;
			case DOMINANT7: // I III V bVII
				semitonePattern = { 0, 4, 7, 10 };
				break;
			case MINOR7: // I iii V bVII
				semitonePattern = { 0, 3, 7, 10 };
				break;
			case HALFDIMINISHED7: // I iii bV bVII
				semitonePattern = { 0, 3, 6, 10 };
				break;
			case DIMINISHED7: // I iii bV bbVII
				semitonePattern = { 0, 3, 6, 9 };
				break;
			case NINTH: // I III V bVII IX
				semitonePattern = { 0, 4, 7, 10, 14 };
				break;
			case ELEVENTH: // I III V bVII IX XI
				semitonePattern = { 0, 4, 7, 10, 14, 17 };
				break;
			case THIRTEENTH: // I III V bVII IX XI XIII
				semitonePattern = { 0, 4, 7, 10, 14, 17, 21 };
				break;
			case SUS2: // I II V
				semitonePattern = { 0, 2, 7 };
				break;
			case SUS4: // I IV V
				semitonePattern = { 0, 5, 7 };
				break;
			case ADD9: // I III V IX
				semitonePattern = { 0, 4, 7, 14 };
				break;
			case ADD11: // I III V XI
				semitonePattern = { 0, 4, 7, 17 };
				break;
			case ADD13: // I III V XIII
				semitonePattern = { 0, 4, 7, 21 };
				break;
			case UNRECOGNIZEDCHORD:
				semitonePattern = { };
				break;
			}

			if (semitonePattern.size() == 0 || semitonePattern.size() != intervals.size()) continue;

			//Cases
			// - The intervals are equal: Return
			// - each of the notes is a multiple of 12 (including 0) away from the interval: Return 
			//	 (notes don't need to be the same multiple away)
			// - 
			bool found = true;
			for (int j = 0; j < semitonePattern.size(); j++)
			{
				if (semitonePattern[j] != intervals[j])
				{
					found = false;
					break;
				}
			}

			if (found)
			{
				return (ChordType)type;
			}
		}

		return UNRECOGNIZEDCHORD;
	}

	static std::pair<ChordType, std::vector<int>> findChordOctave(std::vector<int> intervals)
	{
		for (int type = MAJOR; type < UNRECOGNIZEDCHORD; type++)
		{
			std::vector<int> semitonePattern = {};
			switch (type) {
			case MAJOR: // I III V
				semitonePattern = { 0, 4, 7 };
				break;
			case MINOR: // I iii V
				semitonePattern = { 0, 3, 7 };
				break;
			case DIMINISHED: // I iii bV
				semitonePattern = { 0, 3, 6 };
				break;
			case AUGMENTED: // I III #V
				semitonePattern = { 0, 4, 8 };
				break;
			case MAJOR7: // I III V VII
				semitonePattern = { 0, 4, 7, 11 };
				break;
			case DOMINANT7: // I III V bVII
				semitonePattern = { 0, 4, 7, 10 };
				break;
			case MINOR7: // I iii V bVII
				semitonePattern = { 0, 3, 7, 10 };
				break;
			case HALFDIMINISHED7: // I iii bV bVII
				semitonePattern = { 0, 3, 6, 10 };
				break;
			case DIMINISHED7: // I iii bV bbVII
				semitonePattern = { 0, 3, 6, 9 };
				break;
			case NINTH: // I III V bVII IX
				semitonePattern = { 0, 4, 7, 10, 14 };
				break;
			case ELEVENTH: // I III V bVII IX XI
				semitonePattern = { 0, 4, 7, 10, 14, 17 };
				break;
			case THIRTEENTH: // I III V bVII IX XI XIII
				semitonePattern = { 0, 4, 7, 10, 14, 17, 21 };
				break;
			case SUS2: // I II V
				semitonePattern = { 0, 2, 7 };
				break;
			case SUS4: // I IV V
				semitonePattern = { 0, 5, 7 };
				break;
			case ADD9: // I III V IX
				semitonePattern = { 0, 4, 7, 14 };
				break;
			case ADD11: // I III V XI
				semitonePattern = { 0, 4, 7, 17 };
				break;
			case ADD13: // I III V XIII
				semitonePattern = { 0, 4, 7, 21 };
				break;
			case UNRECOGNIZEDCHORD:
				semitonePattern = { };
				break;
			}

			if (semitonePattern.size() == 0) continue;

			//Cases
			// - The intervals are equal: Return
			// - each of the notes is a multiple of 12 (including 0) away from the interval: Return 
			//	 (notes don't need to be the same multiple away)
			// - 
			bool found = true;
			auto semitonesModded = semitonePattern;

			for (auto& semitone : semitonesModded)
			{
				semitone = negativeMod(semitone, 12);
			}

			for (int j = 0; j < intervals.size(); j++)
			{
				if (std::find(semitonesModded.begin(), semitonesModded.end(), intervals[j] % 12) == semitonesModded.end())
				{
					found = false;
					break;
				}

			}

			if (found)
			{
				return { (ChordType)type, {} };
			}
		}

		return { UNRECOGNIZEDCHORD, {} };
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

	// |-|-|-|-|-|-|-|-|-|-| IINTERVALS |-|-|-|-|-|-|-|-|-|-|-|-|

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
			int newInterval = getSemitoneDistance(start, pitches[i]);
			//int newInterval = negativeMod((negativeMod(pitches[i].note, 12) - negativeMod(start.note, 12)), 12);
			intervals.push_back(newInterval);
		}
		return intervals;
	}

	static std::string getIntervals(PitchEnum pitch, KeySignature key)
	{
		auto distance = getSemitoneDistance(key.root, pitch);
		distance = negativeMod(distance, 12);

		switch (distance)
		{
		case 0:
			return "i"; // Unison
			break;
		case 1:
			return "ii"; // Minor second
			break;
		case 2:
			return "II"; // Major second
			break;
		case 3:
			return "iii"; // Minor third
			break;
		case 4:
			return "III"; // Major third
			break;
		case 5:
			return "IV"; // Perfect fourth
			break;
		case 6:
			return "IV+"; // Tritone
			break;
		case 7:
			return "V"; // Perfect fifth
			break;
		case 8:
			return "vi"; // Minor sixth
			break;
		case 9:
			return "VI"; // Major sixth
			break;
		case 10:
			return "vii"; // Minor seventh
			break;
		case 11:
			return "VII"; // Major seventh
			break;
		default:
			return "error";
		}
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
		distance = distance % 12;
		
		return (distance);

	}

	static int getSemitonesFromMiddleC(Pitch pitch)
	{
		pitch.note = (PitchEnum)negativeMod(pitch.note, 12);

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
		{DIMINISHED, "Diminished"},
		{AUGMENTED, "Augmented"},
		{MAJOR7, "Major7"},
		{DOMINANT7, "Dominant7"},
		{MINOR7, "Minor7"},
		{HALFDIMINISHED7, "Half-Diminished7"},
		{DIMINISHED7, "Diminished7"},
		{NINTH, "Ninth"},
		{ELEVENTH, "Eleventh"},
		{THIRTEENTH, "Thirteenth"},
		{SUS2, "Suspended2"},
		{SUS4, "Suspended4"},
		{ADD9, "Add9"},
		{ADD11, "Add11"},
		{ADD13, "Add13"},
		{UNRECOGNIZEDCHORD, "Unrecognized"},
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