#include "SheetMusicNoteDisplayStaff.h"

SheetMusicNoteDisplayStaff::SheetMusicNoteDisplayStaff()
	: SheetMusicStaff()
{
	hideTimeSignature_ = true;
	updateMeasureStarts();
}

SheetMusicNoteDisplayStaff::SheetMusicNoteDisplayStaff(
	float x, float y, float width, float height, Clef clefType, KeySignature key)
	: SheetMusicStaff(x, y, width, height, clefType, key, TimeSignature{ 4, 4 })
{
	hideTimeSignature_ = true;
	updateMeasureStarts();
}

SheetMusicNoteDisplayStaff::SheetMusicNoteDisplayStaff(
	const RectSpec& rectSpec,
	const MarginSpec& marginSpec,
	Clef clefType,
	KeySignature key)
	: SheetMusicStaff(rectSpec, marginSpec, clefType, key, TimeSignature{ 4, 4 })
{
	hideTimeSignature_ = true;
	updateMeasureStarts();
}

SheetMusicMeasure* SheetMusicNoteDisplayStaff::addMeasure(SheetMusicMeasure* measure)
{
	SheetMusicMeasure* m = SheetMusicStaff::addMeasure(measure);
	if (m != nullptr)
	{
		m->setSingleBeatMeasure(true);
	}
	return m;
}

SheetMusicMeasure* SheetMusicNoteDisplayStaff::addMeasure()
{
	SheetMusicMeasure* m = SheetMusicStaff::addMeasure();
	if (m != nullptr)
	{
		m->setSingleBeatMeasure(true);
	}
	return m;
}

float SheetMusicNoteDisplayStaff::addNote(Note note, float beat)
{
	(void)beat;
	if (measures.empty())
	{
		addMeasure();
	}

	SheetMusicMeasure* m = measures[0];
	m->clearRests();
	m->addNote(note, 0.0f);
	m->addRests();
	m->reload();
	relayoutMeasuresWithWrap();
	colorUpdate();
	return MusicUtilities::getBeats(timeSignature, note.value);
}
