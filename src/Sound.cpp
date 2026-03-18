#include "HeaderFiles/Sound.h"

Sound::Sound(FMOD::System* sys, const std::string& filePath) : 
    system(sys), path(filePath), donePlaying(false), releaseOnStop(true)
{
    sound = nullptr;
    channel = nullptr;

    duration = -1;
    fadeIn = -1;
    fadeOut = -1;

    volume = 1.0;
    maxVolume = 1.0;
    playing = false;

    FMOD_VECTOR initialPosition = { 0.0f, 1.0f, 0.0f }; // Example: 10 units to the left
    position = initialPosition;

    loadSound();

}

Sound::Sound(FMOD::System* sys, FMOD::Sound* sound, double duration, double fadeIn, double fadeOut)
    : system(sys), sound(sound), duration(duration), fadeIn(fadeIn), fadeOut(fadeOut), maxVolume(1.0), releaseOnStop(false)
{
    donePlaying = false;
    playing = false;
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
    : system(sys), path(filePath), duration(duration), fadeIn(fadeIn), fadeOut(fadeOut), maxVolume(1.0), releaseOnStop(true)
{
    donePlaying = false;
    playing = false;
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
    : system(sys), path(filePath), duration(duration), fadeIn(fadeIn), fadeOut(fadeOut), maxVolume(maxVolume), releaseOnStop(true)
{
    donePlaying = false;
    playing = false;
    channel = nullptr;
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

void Sound::Update()
{
    bool shouldStop = false;
    {
        std::lock_guard<std::mutex> lock(stateMutex);

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
        
        channel->isPlaying(&playing);

        // If the sound has been playing for longer than duration, defer Stop()
        // until after we release this mutex to avoid recursive lock deadlock.
        if (playing && duration != -1 && playDuration >= duration)
        {
            shouldStop = true;
        }

        if (!shouldStop)
        {
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
                channel->setVolume((float)volume);

                bool playing;
                channel->isPlaying(&playing);

                if (channel && playing) {
                    FMOD_RESULT result = channel->set3DAttributes(&position, NULL);
                    if (result != FMOD_OK) {
                        std::cerr << "Error setting channel 3D attributes: " << result << std::endl;
                    }
                }
            }   
        }
    }

    if (shouldStop)
    {
        Stop();
        return;
    }

    //std::cout << "\t\tVolume: " << volume << "\n";
}

void Sound::Play()
{
    std::lock_guard<std::mutex> lock(stateMutex);

    if (playbackStarted) {
        return;
    }

    // Consume deferred play request and configure startup volume.
    playRequested = false;
    playing = true;
    playbackStarted = true;
    volume = (fadeIn > 0) ? 0.0 : maxVolume;

    result = system->playSound(sound, nullptr, true, &channel);
    if (result != FMOD_OK) {
        printf("Error playing sound: (%d)\n", result);
        playing = false;
        playbackStarted = false;
        donePlaying = true;
        // Release the sound object only if it's a non-recoverable error
        // sound->release(); 
        return;
    }

    // Verify channel is valid before setting properties
    if (channel) {
        result = channel->setVolume((float)volume);
        result = channel->set3DAttributes(&position, nullptr);
        float minDistance = 0.5f;
        float maxDistance = 1000.0f;
        result = channel->set3DMinMaxDistance(minDistance, maxDistance);

        channel->setPaused(false);
    } else {
        // Handle the case where channel is still null
    }

    startTime = std::chrono::steady_clock::now();
}

void Sound::Stop()
{
    std::lock_guard<std::mutex> lock(stateMutex);

    playing = false;
    donePlaying = true;

    // Stop the sound
    if (channel != nullptr)
    {
        channel->stop();
    }

    //If the FMOD::Sound belongs to the cache rather this Sound
    if (releaseOnStop)
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
    std::lock_guard<std::mutex> lock(stateMutex);

    this->maxVolume = volume;
    // State-only setter: mixer thread applies this to FMOD channel in Update().
    if (fadeIn <= 0.0)
    {
        this->volume = volume;
    }
}

void Sound::setPosition(FMOD_VECTOR pos)
{
    std::lock_guard<std::mutex> lock(stateMutex);

    // State-only setter: mixer thread applies this to FMOD channel in Update().
    this->position = pos;
}

void Sound::Mute()
{
    std::lock_guard<std::mutex> lock(stateMutex);
    isMuted = true;
}


void Sound::Unmute()
{
    std::lock_guard<std::mutex> lock(stateMutex);
    isMuted = false;
}

