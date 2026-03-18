#include "HeaderFiles/SpacialSound.h"

SpacialSound::SpacialSound(FMOD::System* sys, const std::string& filePath, FMOD_VECTOR position)
    : Sound(sys, filePath), position(position)
{
    Sound::setPosition(position);
}

SpacialSound::SpacialSound(FMOD::System* sys, const std::string& filePath, double duration, double fadeIn, double fadeOut, FMOD_VECTOR position)
    : Sound(sys, filePath, duration, fadeIn, fadeOut), position(position)
{
    Sound::setPosition(position);
}

SpacialSound::SpacialSound(FMOD::System* sys, const std::string& filePath, double duration, double fadeIn, double fadeOut, double maxVolume, FMOD_VECTOR position)
    : Sound(sys, filePath, duration, fadeIn, fadeOut, maxVolume), position(position)
{
    Sound::setPosition(position);
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
    Sound::setPosition(newPosition);

    if (channel != nullptr) {
        result = channel->set3DAttributes(&position, NULL);
    }
}