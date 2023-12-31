#pragma once

#include "Sound.h"


class SpacialSound : Sound
{
private:
	FMOD_VECTOR position =	{ 0.0f, 0.0f, 0.0f };

public:
	SpacialSound();
	SpacialSound(FMOD::System* sys, const std::string& filePath, FMOD_VECTOR position);
	SpacialSound(FMOD::System* sys, const std::string& filePath, double duration, double fadeIn, double fadeOut, FMOD_VECTOR position);
	SpacialSound(FMOD::System* sys, const std::string& filePath, double duration, double fadeIn, double fadeOut, double maxVolume, FMOD_VECTOR position);
	
	void load3DSound();


	// overridden from Sound
	void Play() override;

	//getters and setters
	void setPosition(FMOD_VECTOR newPosition);
};

