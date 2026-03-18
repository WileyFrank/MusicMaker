#include <vector>
#include <chrono>
#include <thread>
#include "HeaderFiles/SoundMixer.h"


SoundMixer::SoundMixer() : system(nullptr), running(false), nextHandle(1) {
}

void SoundMixer::mixerThreadFunction() {
    using clock = std::chrono::steady_clock;
    const auto tickDuration = std::chrono::milliseconds(10); // ~100 Hz audio update

    while (running) {
        auto start = clock::now();

        Update(); // perform one audio tick

        auto end = clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        if (elapsed < tickDuration) {
            std::this_thread::sleep_for(tickDuration - elapsed);
        }
        // If Update took longer than tickDuration we immediately continue
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

void SoundMixer::cleanupBuffer() {
	std::lock_guard<std::mutex> lock(bufferMutex); // Lock if accessing from multiple threads

	for (auto it = soundBuffer.begin(); it != soundBuffer.end(); ) {
        Sound* sound = *it;
		if (sound->isDonePlaying()) {
            auto handleIt = soundToHandle.find(sound);
            if (handleIt != soundToHandle.end())
            {
                handleToSound.erase(handleIt->second);
                soundToHandle.erase(handleIt);
            }
			delete sound; // Delete the Sound object
			it = soundBuffer.erase(it); // Erase from vector and get the next iterator
		}
		else {
			++it; // Move to next element
		}
	}
}

bool SoundMixer::isPlaying()
{
    std::vector<Sound*> activeSnapshot;
    {
        std::lock_guard<std::mutex> lock(bufferMutex);
        activeSnapshot = soundBuffer;
    }

    for (Sound* sound : activeSnapshot)
    {
        if (sound != nullptr && sound->isPlaying())
        {
            return true;
        }
    }
	return false;
}

void SoundMixer::Update()
{
	//New sounds are added in the add methods to the buffer
	//finished sounds are periodically removed

	// Snapshot sounds from buffer under lock so we can safely update without
    // holding the mutex for the entire audio tick.
    {
        std::lock_guard<std::mutex> lock(bufferMutex);
        soundVector = soundBuffer;
    }

	//updating sounds (pitch, volume, etc)
	for (Sound* sound : soundVector)
	{
        // Only the mixer thread starts playback
        if (sound->shouldStartPlayback())
        {
            sound->Play();
        }
		sound->Update();
	}

    // Periodically clean up finished sounds
    cleanupBuffer();
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

SoundMixer::SoundHandle SoundMixer::playSound(std::string path)
{
    Sound* newSound = new Sound(system, path);
    newSound->requestPlay();

    std::lock_guard<std::mutex> lock(bufferMutex);
    SoundHandle handle = nextHandle.fetch_add(1);
    soundBuffer.push_back(newSound);
    handleToSound[handle] = newSound;
    soundToHandle[newSound] = handle;
    return handle;
}

SoundMixer::SoundHandle SoundMixer::playSound(FMOD::Sound* sound, double duration, double fadeIn, double fadeOut)
{
    Sound* newSound = new Sound(system, sound, duration, fadeIn, fadeOut);
    newSound->requestPlay();

    std::lock_guard<std::mutex> lock(bufferMutex);
    SoundHandle handle = nextHandle.fetch_add(1);
    soundBuffer.push_back(newSound);
    handleToSound[handle] = newSound;
    soundToHandle[newSound] = handle;
    return handle;
}

SoundMixer::SoundHandle SoundMixer::playSound(const std::string& path, double duration, double fadeIn, double fadeOut)
{
    Sound* newSound = new Sound(system, path, duration, fadeIn, fadeOut);
    newSound->requestPlay();

    std::lock_guard<std::mutex> lock(bufferMutex);
    SoundHandle handle = nextHandle.fetch_add(1);
    soundBuffer.push_back(newSound);
    handleToSound[handle] = newSound;
    soundToHandle[newSound] = handle;
    return handle;
}

SoundMixer::SoundHandle SoundMixer::playSound(const std::string& path, double duration, double fadeIn, double fadeOut, double maxVolume)
{
    Sound* newSound = new Sound(system, path, duration, fadeIn, fadeOut, maxVolume);
    newSound->requestPlay();

    std::lock_guard<std::mutex> lock(bufferMutex);
    SoundHandle handle = nextHandle.fetch_add(1);
    soundBuffer.push_back(newSound);
    handleToSound[handle] = newSound;
    soundToHandle[newSound] = handle;
    return handle;
}

bool SoundMixer::setSoundVolume(SoundHandle handle, float volume)
{
    std::lock_guard<std::mutex> lock(bufferMutex);
    auto it = handleToSound.find(handle);
    if (it == handleToSound.end())
    {
        return false;
    }
    it->second->setVolume(volume);
    return true;
}

bool SoundMixer::setSoundPosition(SoundHandle handle, FMOD_VECTOR vec)
{
    std::lock_guard<std::mutex> lock(bufferMutex);
    auto it = handleToSound.find(handle);
    if (it == handleToSound.end())
    {
        return false;
    }
    it->second->setPosition(vec);
    return true;
}

bool SoundMixer::stopSound(SoundHandle handle)
{
    std::lock_guard<std::mutex> lock(bufferMutex);
    auto it = handleToSound.find(handle);
    if (it == handleToSound.end())
    {
        return false;
    }
    it->second->Stop();
    return true;
}

bool SoundMixer::isSoundActive(SoundHandle handle)
{
    std::lock_guard<std::mutex> lock(bufferMutex);
    return handleToSound.find(handle) != handleToSound.end();
}

