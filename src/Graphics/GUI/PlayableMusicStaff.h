#pragma once
#include "RenderObject.h"
#include "../../HeaderFiles/SoundMixer.h"
#include "../SheetMusic/SheetMusicStaff.h"


// |-|-|-|-|-|-|-|-|-|-|-|-| Audio |-|-|-|-|-|-|-|-|-|-|-|-|-|-|
/*
* Audio will be handled by the mixer,
* which is to be passed in using setMixer()
*
*/

class PlayableMusicStaff
	: public RenderObject
{
	SheetMusicStaff staff;
	SoundMixer& mixer;

	//Buttons for play and pause, slider beneath the bars
	float currentBeat;


};

