#pragma once

#include "SheetMusicStaff.h"

// Clef + key signature + one beat of music (no time signature). Multiple notes added
// at the same attack stack as a chord; uses the same rendering path as SheetMusicStaff.
class SheetMusicNoteDisplayStaff : public SheetMusicStaff
{
public:
	SheetMusicNoteDisplayStaff();
	SheetMusicNoteDisplayStaff(float x, float y, float width, float height, Clef clefType,
		KeySignature key = KeySignature());
	SheetMusicNoteDisplayStaff(
		const RectSpec& rectSpec,
		const MarginSpec& marginSpec,
		Clef clefType,
		KeySignature key = KeySignature());

	~SheetMusicNoteDisplayStaff() override = default;

	SheetMusicMeasure* addMeasure(SheetMusicMeasure* measure) override;
	SheetMusicMeasure* addMeasure() override;
	float addNote(Note note, float beat) override;
};
