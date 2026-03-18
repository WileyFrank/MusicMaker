#pragma once
#include <iostream>
#include <fmod.hpp>
#include <chrono>
#include <thread>
#include <string>
#include <mutex>


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

	bool playing;
	bool donePlaying;
	bool releaseOnStop;

	FMOD_VECTOR position;
	mutable std::mutex stateMutex;

	// Playback request/state flags used by the audio thread
	bool playRequested = false;
	bool playbackStarted = false;


public:
	Sound(FMOD::System* sys, const std::string& filePath);
	Sound(FMOD::System* sys, FMOD::Sound* sound, double duration, double fadeIn, double fadeOut);
	Sound(FMOD::System* sys, const std::string& filePath, double duration, double fadeIn, double fadeOut);
	Sound(FMOD::System* sys, const std::string& filePath, double duration, double fadeIn, double fadeOut, double maxVolume);

	~Sound();

	void loadSound();

	void Update();

	//Playing
	virtual void Play();
	// Called from non-audio threads to ask the audio thread to start playback
	void requestPlay() {
		std::lock_guard<std::mutex> lock(stateMutex);
		playRequested = true;
	}
	void Stop();

	//Volume
	void setVolume(float volume);
	void setPosition(FMOD_VECTOR pos);

	//Muting
	void Mute();
	void Unmute();

	// Used by the mixer thread to decide when to call Play()
	bool shouldStartPlayback() {
		std::lock_guard<std::mutex> lock(stateMutex);
		return playRequested && !playbackStarted;
	}

	bool isPlaying() {
		std::lock_guard<std::mutex> lock(stateMutex);
		// Consider both queued-start and actively playing states as "playing".
		return playRequested || (playing && !donePlaying);
	}
	bool isDonePlaying() const {
		std::lock_guard<std::mutex> lock(stateMutex);
		return donePlaying;
	}
};

