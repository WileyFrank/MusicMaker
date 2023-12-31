#include "HeaderFiles/Sound.h"

Sound::Sound(FMOD::System* sys, const std::string& filePath) : 
    system(sys), path(filePath), donePlaying(false)
{
    sound = NULL;
    channel = NULL;
    channel->setVolume((float)volume);

    duration = -1;
    fadeIn = -1;
    fadeOut = -1;

    volume = 1.0;

    loadSound();

}

Sound::Sound(FMOD::System* sys, const std::string& filePath, double duration, double fadeIn, double fadeOut)
    : system(sys), path(filePath), duration(duration), fadeIn(fadeIn), fadeOut(fadeOut), donePlaying(false), maxVolume(1.0)
{

    sound = NULL;
    channel = NULL;
    channel->setVolume((float)volume);

    if (fadeIn > 0)
    {
        volume = 0.0;
    }
    else
    {
        volume = 1.0;
    }


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
    if (donePlaying)
    {
        channel->setVolume(0);
        return;
    }
    if (startTime == std::chrono::steady_clock::time_point{})
    {
        //if sound has not initialized yet
        return;
    }

    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedDuration = currentTime - startTime;

    auto playDuration = std::chrono::duration<double>(elapsedDuration).count();


    std::cout << "Note:\n";
    std::cout << "Play Duration: " << playDuration;

    bool playing;
    channel->isPlaying(&playing);

    //If the sound has been playing for longer than the duration provided.
    if (duration != -1 && playDuration >= duration && playing)
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

    channel->setVolume((float)(volume));

    std::cout << "\t\tVolume: " << volume << "\n";
}

void Sound::Play()
{
    playing = true;

    result = system->playSound(sound, 0, false, &channel);
    if (result != FMOD_OK) {
        printf("Error playing sound: (%d)\n", result);
        sound->release();  // Release the sound object
        return;
    }

    startTime = std::chrono::steady_clock::now();
}

void Sound::Stop()
{
    playing = false;
    donePlaying = true;

    // Stop the sound
    channel->stop();

    sound->release();
}

void Sound::loadSound()
{
    sound = NULL;
    channel = NULL;
    channel->setVolume((float)volume);



    result = system->createSound(path.c_str(), FMOD_DEFAULT, 0, &sound);
    if (result != FMOD_OK) {
        printf("Error loading sound: (%d)\n", result);
        return;
    }
}

void Sound::setVolume(float volume)
{
    this->volume = volume;
    channel->setVolume((float)(volume));
}

void Sound::Mute()
{
    isMuted = true;
}


void Sound::Unmute()
{
    isMuted = false;
}

