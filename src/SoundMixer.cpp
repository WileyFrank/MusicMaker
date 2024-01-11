#include <vector>
#include "HeaderFiles/SoundMixer.h"


SoundMixer::SoundMixer() : system(nullptr), running(false) {
	
}

void SoundMixer::mixerThreadFunction() {
	while (running) {
		Update(); // Call the update method or any other processing
	}
}

void SoundMixer::start() {
	if (!running) {
		running = true;
		mixerThread = std::thread(&SoundMixer::mixerThreadFunction, this);
	}
}

void SoundMixer::stop()
{
	if (running) {
		running = false;
		if (mixerThread.joinable()) {
			mixerThread.join();
		}
	}
}

void SoundMixer::processPendingChanges()
{
	std::lock_guard<std::mutex> lock(bufferMutex);
}

void SoundMixer::cleanupBuffer() {
	std::lock_guard<std::mutex> lock(bufferMutex); // Lock if accessing from multiple threads

	for (auto it = soundBuffer.begin(); it != soundBuffer.end(); ) {
		if ((*it)->isDonePlaying()) {
			delete* it; // Delete the Sound object
			it = soundBuffer.erase(it); // Erase from vector and get the next iterator
		}
		else {
			++it; // Move to next element
		}
	}
}

bool SoundMixer::isPlaying()
{
	return true;
}

void SoundMixer::Update()
{
	//New sounds are added in the add methods to the buffer
	//finished sounds are periodically removed
	
	//maintaining sound vector from buffer
	if (soundBuffer.size() > 0)
	{
		soundVector = soundBuffer;
	}
	else
	{
		auto emptied = soundVector.empty();
	}

	//updating sounds (pitch, volume, etc)
	for (Sound* sound : soundVector)
	{
		sound->Update();
	}

	updateCount++;

	if (updateCount % 1 == 0)
	{
		updateCount = 0;
		cleanupBuffer();
	}
	if (system != nullptr)
	{
		system->update();
	}
	return;
}

void SoundMixer::setSystem(FMOD::System* system)
{
	this->system = system;
}

Sound& SoundMixer::addSound(std::string path)
{
	Sound* newSound = new Sound(system, path);
	{
		std::lock_guard<std::mutex> lock(bufferMutex);
		soundBuffer.push_back(newSound);
	}
	newSound->Play();

	return *newSound;
}

Sound& SoundMixer::addSound(FMOD::Sound* sound, double duration, double fadeIn, double fadeOut)
{
	Sound* newSound = new Sound(system, sound, duration, fadeIn, fadeOut);
	newSound->Play();

	std::lock_guard<std::mutex> lock(bufferMutex);
	soundBuffer.push_back(newSound);
	this->soundBuffer = soundBuffer;

	return *newSound;
}

Sound& SoundMixer::addSound(const std::string& path, double duration, double fadeIn, double fadeOut)
{
	Sound* newSound = new Sound(system, path, duration, fadeIn, fadeOut);
	newSound->Play();

	std::lock_guard<std::mutex> lock(bufferMutex);
	soundBuffer.push_back(newSound);
	this->soundBuffer = soundBuffer;

	return *newSound;
}


Sound& SoundMixer::addSound(const std::string& path, double duration, double fadeIn, double fadeOut, double maxVolume)
{
	Sound* newSound = new Sound(system, path, duration, fadeIn, fadeOut, maxVolume);
	newSound->Play();
	
	std::lock_guard<std::mutex> lock(bufferMutex);
	this->soundBuffer.push_back(newSound);
	return *newSound;
}

