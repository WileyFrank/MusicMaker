#include "HeaderFiles/Sound.h"

Sound::Sound(FMOD::System* sys, const std::string& filePath) : 
    system(sys), path(filePath), donePlaying(false)
{
    sound = nullptr;
    channel = nullptr;

    duration = -1;
    fadeIn = -1;
    fadeOut = -1;

    volume = 1.0;

    FMOD_VECTOR initialPosition = { 0.0f, 1.0f, 0.0f }; // Example: 10 units to the left
    position = initialPosition;

    loadSound();

}

Sound::Sound(FMOD::System* sys, FMOD::Sound* sound, double duration, double fadeIn, double fadeOut)
    : system(sys), sound(sound), duration(duration), fadeIn(fadeIn), fadeOut(fadeOut), donePlaying(false), maxVolume(1.0)
{
    channel = nullptr;

    if (fadeIn > 0)
    {
        volume = 0.0;
    }
    else
    {
        volume = 1.0;
    }
    FMOD_VECTOR initialPosition = { 0.0f, 1.0f, 0.0f }; // Example: 10 units to the left
    position = initialPosition;
}

Sound::Sound(FMOD::System* sys, const std::string& filePath, double duration, double fadeIn, double fadeOut)
    : system(sys), path(filePath), duration(duration), fadeIn(fadeIn), fadeOut(fadeOut), donePlaying(false), maxVolume(1.0)
{

    channel = nullptr;

    if (fadeIn > 0)
    {
        volume = 0.0;
    }
    else
    {
        volume = 1.0;
    }
    FMOD_VECTOR initialPosition = { 0.0f, 1.0f, 0.0f }; // Example: 10 units to the left
    position = initialPosition;

    loadSound();

}


Sound::Sound(FMOD::System* sys, const std::string& filePath, double duration, double fadeIn, double fadeOut, double maxVolume)
    : system(sys), path(filePath), duration(duration), fadeIn(fadeIn), fadeOut(fadeOut), donePlaying(false), maxVolume(maxVolume)
{
    if (fadeIn > 0)
    {
        volume = 0.0;
    }
    else
    {
        volume = maxVolume;
    }
    FMOD_VECTOR initialPosition = { 0.0f, 1.0f, 0.0f }; // Example: 10 units to the left
    position = initialPosition;
    loadSound();

}

Sound::~Sound()
{
    if (playing)
    {
        Stop();
    }
}

Sound::Sound()
{
    return;
}

void Sound::Update()
{
    if (startTime == std::chrono::steady_clock::time_point{} || !playing || channel == nullptr) {
        return;
    }

    if (donePlaying)
    {
        channel->setVolume(0);
        return;
    }

    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedDuration = currentTime - startTime;

    auto playDuration = std::chrono::duration<double>(elapsedDuration).count();


    /*std::cout << "Note:\n";
    std::cout << "Play Duration: " << playDuration;*/

    bool playing;
    channel->isPlaying(&playing);

    //If the sound has been playing for longer than the duration provided.
    if (playing && duration != -1 && playDuration >= duration)
    {
        Stop();
        return;
    }

    if (isMuted)
    {
        volume = 0.0;
        channel->setVolume((float)volume);
        return;
    }
    
    double startVolume = 1.0;
    //updating volume for fade in & fade out
    if (fadeIn != -1) 
    {
        if (playDuration <= fadeIn )
        {
            startVolume = (playDuration / fadeIn);
        }
    }
    if (fadeOut != -1)
    {
        if (playDuration >= duration - fadeOut)
        {
            startVolume = ((duration - playDuration) / fadeOut) ;
        }
    }

    startVolume *= maxVolume;

    volume = startVolume;

    if (channel)
    {
        channel->setVolume(volume);

        FMOD_RESULT result = channel->set3DAttributes(&position, NULL);
        if (result != FMOD_OK) {
            std::cerr << "Error setting channel 3D attributes: " << result << std::endl;
        }
    }

    //std::cout << "\t\tVolume: " << volume << "\n";
}

void Sound::Play()
{
    playing = true;

    result = system->playSound(sound, nullptr, true, &channel);
    if (result != FMOD_OK) {
        printf("Error playing sound: (%d)\n", result);
        // Release the sound object only if it's a non-recoverable error
        // sound->release(); 
        return;
    }

    // Verify channel is valid before setting properties
    if (channel) {
        result = channel->setVolume((float)volume);
        result = channel->set3DAttributes(&position, nullptr);

        channel->setPaused(false);
    } else {
        // Handle the case where channel is still null
    }

    startTime = std::chrono::steady_clock::now();
}

void Sound::Stop()
{
    playing = false;
    donePlaying = true;
    stopped = true;

    // Stop the sound
    channel->stop();

    sound->release();
}

void Sound::loadSound()
{
    channel = NULL;

    result = system->createSound(path.c_str(), FMOD_3D, nullptr, &sound);
    if (result != FMOD_OK) {
        printf("Error loading sound: (%d)\n", result);
        return;
    }
    
    setPosition(position);

}

void Sound::setVolume(float volume)
{
    this->maxVolume = volume;
    channel->setVolume((float)(volume));
}

void Sound::setPosition(FMOD_VECTOR pos)
{
    this->position = pos; 
    channel->set3DAttributes(&position, nullptr);
    float panLevel = 0.75f; // Adjust this value between 0.0 (full mono) and 1.0 (full 3D) as needed

    // After initializing and playing the sound
    float minDistance = 0.5f; // Minimum distance at which the sound is at its loudest
    float maxDistance = 1000.0f; // Maximum distance at which the sound stops attenuating

    result = channel->set3DMinMaxDistance(minDistance, maxDistance);
    /*FMOD_VECTOR vec = { 0.0f, 1.0f, 0.0f };
    result = channel->set3DConeOrientation(&vec);
    if (result != FMOD_OK) {
        std::cerr << "Error setting 3D min/max distance: " << result << std::endl;
    }*/
}

void Sound::Mute()
{
    isMuted = true;
}


void Sound::Unmute()
{
    isMuted = false;
}

