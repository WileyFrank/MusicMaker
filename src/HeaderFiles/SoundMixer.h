#pragma once

#include <string>
#include <vector>
#include <thread>
#include "HeaderFiles/Sound.h"
#include <mutex>


class SoundMixer {
private:
    std::vector<Sound*> soundVector;     // Active sounds
    std::vector<Sound*> pendingChanges;   // Pending additions or deletions
    std::vector<Sound*> soundBuffer;   // Buffer

    std::mutex bufferMutex;

    FMOD::System* system;

    std::thread mixerThread;
    //mixer running - active and able to play sounds, different to playing - actively playing sound
    //atomic is for thread safety
    std::atomic<bool> running;

    int updateCount;


    // Private Constructor
    SoundMixer();

    SoundMixer(const SoundMixer&) = delete;
    SoundMixer& operator=(const SoundMixer&) = delete;



public:

    //Thread control methods
    void start();
    void stop();

    void mixerThreadFunction();
    void processPendingChanges();
    void cleanupBuffer();


    // Public Method to get the instance of the class
    static SoundMixer& getInstance() {
        static SoundMixer instance; // Guaranteed to be destroyed and instantiated on first use

        return instance;
    }

    bool isPlaying();

    void Update();

    void setSystem(FMOD::System* system);

    void addSound(std::string path);
    void addSound(const std::string& path, double duration, double fadeIn, double fadeOut);
    void addSound(const std::string& path, double duration, double fadeIn, double fadeOut, double maxVolume);
};