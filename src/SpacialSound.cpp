#include "HeaderFiles/SpacialSound.h"

SpacialSound::SpacialSound()
{
}

SpacialSound::SpacialSound(FMOD::System* sys, const std::string& filePath, FMOD_VECTOR position)
{
    system = sys;
    path = filePath;
    donePlaying = false;

    duration = -1;
    fadeIn = -1;
    fadeOut = -1;

    volume = 1.0;

    load3DSound();

}

SpacialSound::SpacialSound(FMOD::System* sys, const std::string& filePath, double duration, double fadeIn, double fadeOut, FMOD_VECTOR position)
{
    this->system = sys;
    this->path = filePath;
    this->donePlaying = false;

    this->duration = duration;
    this->fadeIn = fadeIn;
    this->fadeOut = fadeOut;

    if (fadeIn > 0)
    {
        volume = 0.0;
    }
    else
    {
        volume = 1.0;
    }


    load3DSound();

}

SpacialSound::SpacialSound(FMOD::System* sys, const std::string& filePath, double duration, double fadeIn, double fadeOut, double maxVolume, FMOD_VECTOR position)
{
}

void SpacialSound::load3DSound()
{
    sound = NULL;
    channel = NULL;
    channel->setVolume((float)volume);



    result = system->createSound(path.c_str(), FMOD_3D, 0, &sound);
    if (result != FMOD_OK) {
        printf("Error loading sound: (%d)\n", result);
        return;
    }
}

void SpacialSound::Play()
{
    playing = true;

    result = system->playSound(sound, 0, false, &channel);
    if (result != FMOD_OK) {
        printf("Error playing sound: (%d)\n", result);
        sound->release();  // Release the sound object
        return;
    }

    result = channel->set3DAttributes(&position, NULL);

    startTime = std::chrono::steady_clock::now();
}

void SpacialSound::setPosition(FMOD_VECTOR newPosition)
{
    this->position = newPosition;
    result = channel->set3DAttributes(&position, NULL);
}