#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <thread>
#include <atomic>
#include "Sound.h"
#include <mutex>


class SoundMixer {
public:
    using SoundHandle = std::uint64_t;

private:
    std::vector<Sound*> soundVector;     // Active sounds
    std::vector<Sound*> soundBuffer;   // Buffer
    std::unordered_map<SoundHandle, Sound*> handleToSound;
    std::unordered_map<Sound*, SoundHandle> soundToHandle;

    std::mutex bufferMutex;

    FMOD::System* system;

    std::thread mixerThread;
    //mixer running - active and able to play sounds, different to playing - actively playing sound
    //atomic is for thread safety
    std::atomic<bool> running;
    std::atomic<SoundHandle> nextHandle;


    // Private Constructor
    SoundMixer();

    SoundMixer(const SoundMixer&) = delete;
    SoundMixer& operator=(const SoundMixer&) = delete;


public:

    //Thread control methods
    void start();
    void stop();

    void mixerThreadFunction();
    void cleanupBuffer();


    // Public Method to get the instance of the class
    static SoundMixer& getInstance() {
        static SoundMixer instance; // Guaranteed to be destroyed and instantiated on first use

        return instance;
    }

    bool isPlaying();

    void Update();

    void setSystem(FMOD::System* system);

    // Stable-handle API (preferred)
    SoundHandle playSound(std::string path);
    SoundHandle playSound(FMOD::Sound* sound, double duration, double fadeIn, double fadeOut);
    SoundHandle playSound(const std::string& path, double duration, double fadeIn, double fadeOut);
    SoundHandle playSound(const std::string& path, double duration, double fadeIn, double fadeOut, double maxVolume);
    bool setSoundVolume(SoundHandle handle, float volume);
    bool setSoundPosition(SoundHandle handle, FMOD_VECTOR vec);
    bool stopSound(SoundHandle handle);
    bool isSoundActive(SoundHandle handle);

};