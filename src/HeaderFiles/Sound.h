#pragma once
#include <iostream>
#include <fmod.hpp>
#include <chrono>
#include <thread>
#include <string>


class Sound
{
protected:
	FMOD_RESULT result;
	FMOD::Sound* sound;
	FMOD::Channel* channel;
	FMOD::System* system;

	std::string path;

	//duration and time

	std::chrono::steady_clock::time_point startTime;

	double duration;
	double fadeIn;
	double fadeOut;
	double maxVolume;

	//Volume and mute
	bool isMuted = false;
	double volume; // 0.0 - 1.0
	double masterVolume; // 0.0 - 1.0

	bool playing;
	bool donePlaying;
	bool stopped;
	bool releaseOnStop;

	FMOD_VECTOR position;


public:

	Sound();

	Sound(FMOD::System* sys, const std::string& filePath);
	Sound(FMOD::System* sys, FMOD::Sound* sound, double duration, double fadeIn, double fadeOut);
	Sound(FMOD::System* sys, const std::string& filePath, double duration, double fadeIn, double fadeOut);
	Sound(FMOD::System* sys, const std::string& filePath, double duration, double fadeIn, double fadeOut, double maxVolume);

	~Sound();

	void loadSound();

	void Update();

	//Playing
	virtual void Play();
	void Stop();

	//Volume
	void setVolume(float volume);
	void setPosition(FMOD_VECTOR pos);

	//Muting
	void Mute();
	void Unmute();

	bool isPlaying() { return true; }
	bool isDonePlaying() const { return donePlaying; }
};

