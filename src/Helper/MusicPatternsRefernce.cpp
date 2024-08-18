//Cmaj I IV V I
/* {


    //Cmaj7
    mixer.addSound("Sounds/Piano/piano-c3.wav", 0.8, 0, 0.35);
    mixer.addSound("Sounds/Piano/piano-e3.wav", 0.8, 0, 0.35);
    mixer.addSound("Sounds/Piano/piano-g3.wav", 0.8, 0, 0.35);
    mixer.addSound("Sounds/Piano/piano-c4.wav", 0.8, 0, 0.35);
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    mixer.addSound("Sounds/Piano/piano-c3.wav", 0.8, 0, 0.35);
    mixer.addSound("Sounds/Piano/piano-e3.wav", 0.8, 0, 0.35);
    mixer.addSound("Sounds/Piano/piano-g3.wav", 0.8, 0, 0.35);
    mixer.addSound("Sounds/Piano/piano-c4.wav", 0.8, 0, 0.35);
    std::this_thread::sleep_for(std::chrono::milliseconds(400));

    //Fmaj7
    mixer.addSound("Sounds/Piano/piano-f3.wav", 0.8, 0, 0.5);
    mixer.addSound("Sounds/Piano/piano-a3.wav", 0.8, 0, 0.5);
    mixer.addSound("Sounds/Piano/piano-c3.wav", 0.8, 0, 0.5);
    mixer.addSound("Sounds/Piano/piano-f4.wav", 0.8, 0, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    mixer.addSound("Sounds/Piano/piano-f3.wav", 0.8, 0, 0.5);
    mixer.addSound("Sounds/Piano/piano-a3.wav", 0.8, 0, 0.5);
    mixer.addSound("Sounds/Piano/piano-c3.wav", 0.8, 0, 0.5);
    mixer.addSound("Sounds/Piano/piano-f4.wav", 0.8, 0, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(400));

    //Gmaj7
    mixer.addSound("Sounds/Piano/piano-g3.wav", 0.8, 0, 0.5);
    mixer.addSound("Sounds/Piano/piano-b3.wav", 0.8, 0, 0.5);
    mixer.addSound("Sounds/Piano/piano-d3.wav", 0.8, 0, 0.5);
    mixer.addSound("Sounds/Piano/piano-g4.wav", 0.8, 0, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    mixer.addSound("Sounds/Piano/piano-g3.wav", 0.8, 0, 0.5);
    mixer.addSound("Sounds/Piano/piano-b3.wav", 0.8, 0, 0.5);
    mixer.addSound("Sounds/Piano/piano-d3.wav", 0.8, 0, 0.5);
    mixer.addSound("Sounds/Piano/piano-g4.wav", 0.8, 0, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(400));

    //Cmaj7
    mixer.addSound("Sounds/Piano/piano-c3.wav", 0.8, 0, 0.35);
    mixer.addSound("Sounds/Piano/piano-e3.wav", 0.8, 0, 0.35);
    mixer.addSound("Sounds/Piano/piano-g3.wav", 0.8, 0, 0.35);
    mixer.addSound("Sounds/Piano/piano-c4.wav", 0.8, 0, 0.35);
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    mixer.addSound("Sounds/Piano/piano-c3.wav", 0.8, 0, 0.35);
    mixer.addSound("Sounds/Piano/piano-e3.wav", 0.8, 0, 0.35);
    mixer.addSound("Sounds/Piano/piano-g3.wav", 0.8, 0, 0.35);
    mixer.addSound("Sounds/Piano/piano-c4.wav", 0.8, 0, 0.35);
    std::this_thread::sleep_for(std::chrono::milliseconds(400));


}*/

//A more Complex Pattern
/*float bpm = 1;
int delayMillis = (int)((bpm / 60) * 1000);
{
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis / 8));


    mixer.addSound("Sounds/Piano/piano-a3.wav", (double)delayMillis / 800, 0, 0.35, 1.0);
    mixer.addSound("Sounds/Piano/piano-c4.wav", (double)delayMillis / 800, 0, 0.35, 1.0);
    mixer.addSound("Sounds/Piano/piano-e4.wav", (double)delayMillis / 800, 0, 0.35, 1.0);


    mixer.addSound("Sounds/Piano/piano-c5.wav", (double)delayMillis/4000, 0, (double)delayMillis/8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis/8));
    mixer.addSound("Sounds/Piano/piano-d5.wav", (double)delayMillis/4000, 0, (double)delayMillis/8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis/8));
    mixer.addSound("Sounds/Piano/piano-e5.wav", (double)delayMillis/4000, 0, (double)delayMillis/8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis/8));
    mixer.addSound("Sounds/Piano/piano-f5.wav", 0.8, 0, 0.35, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis/8));
    mixer.addSound("Sounds/Piano/piano-g5.wav", (double)delayMillis/4000, 0, (double)delayMillis/8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis/8));
    mixer.addSound("Sounds/Piano/piano-a5.wav", (double)delayMillis/4000, 0, (double)delayMillis/8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis/8));
    mixer.addSound("Sounds/Piano/piano-b5.wav", (double)delayMillis/4000, 0, (double)delayMillis/8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis/8));
    mixer.addSound("Sounds/Piano/piano-c6.wav", (double)delayMillis/4000, 0, (double)delayMillis/8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis/8));


    mixer.addSound("Sounds/Piano/piano-f3.wav", (double)delayMillis / 800, 0, 0.35);
    mixer.addSound("Sounds/Piano/piano-a3.wav", (double)delayMillis / 800, 0, 0.35);
    mixer.addSound("Sounds/Piano/piano-c4.wav", (double)delayMillis / 800, 0, 0.35);

    mixer.addSound("Sounds/Piano/piano-b5.wav", (double)delayMillis/4000, 0, (double)delayMillis/8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis / 8));
    mixer.addSound("Sounds/Piano/piano-a5.wav", (double)delayMillis/4000, 0, (double)delayMillis/8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis / 8));
    mixer.addSound("Sounds/Piano/piano-g5.wav", (double)delayMillis/4000, 0, (double)delayMillis/8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis / 8));
    mixer.addSound("Sounds/Piano/piano-f5.wav", (double)delayMillis/4000, 0, (double)delayMillis/8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis / 8));
    mixer.addSound("Sounds/Piano/piano-e5.wav", (double)delayMillis/4000, 0, (double)delayMillis/8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis / 8));
    mixer.addSound("Sounds/Piano/piano-d5.wav", (double)delayMillis/4000, 0, (double)delayMillis/8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis / 8));
    mixer.addSound("Sounds/Piano/piano-c5.wav", (double)delayMillis/4000, 0, (double)delayMillis/8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis / 8));
    mixer.addSound("Sounds/Piano/piano-d5.wav", (double)delayMillis/4000, 0, (double)delayMillis/8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis/8));

    mixer.addSound("Sounds/Piano/piano-c3.wav", 3, 0, 0.35);
    mixer.addSound("Sounds/Piano/piano-e3.wav", 3, 0, 0.35);
    mixer.addSound("Sounds/Piano/piano-g3.wav", 3, 0, 0.35);

    mixer.addSound("Sounds/Piano/piano-e5.wav", 3, 0, 0.35, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis * 2));

    mixer.addSound("Sounds/Piano/piano-a3.wav", (double)delayMillis / 1000, 0, 0.35, 1.0);
    mixer.addSound("Sounds/Piano/piano-c4.wav", (double)delayMillis / 1000, 0, 0.35, 1.0);
    mixer.addSound("Sounds/Piano/piano-e4.wav", (double)delayMillis / 1000, 0, 0.35, 1.0);


    mixer.addSound("Sounds/Piano/piano-c5.wav", (double)delayMillis / 4000, 0, (double)delayMillis / 8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis / 8));
    mixer.addSound("Sounds/Piano/piano-d5.wav", (double)delayMillis / 4000, 0, (double)delayMillis / 8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis / 8));
    mixer.addSound("Sounds/Piano/piano-e5.wav", (double)delayMillis / 4000, 0, (double)delayMillis / 8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis / 8));
    mixer.addSound("Sounds/Piano/piano-f5.wav", 0.8, 0, 0.35, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis / 8));
    mixer.addSound("Sounds/Piano/piano-g5.wav", (double)delayMillis / 4000, 0, (double)delayMillis / 8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis / 8));
    mixer.addSound("Sounds/Piano/piano-a5.wav", (double)delayMillis / 4000, 0, (double)delayMillis / 8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis / 8));
    mixer.addSound("Sounds/Piano/piano-b5.wav", (double)delayMillis / 4000, 0, (double)delayMillis / 8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis / 8));
    mixer.addSound("Sounds/Piano/piano-c6.wav", (double)delayMillis / 4000, 0, (double)delayMillis / 8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis / 8));


    mixer.addSound("Sounds/Piano/piano-f3.wav", (double)delayMillis / 800, 0, 0.35);
    mixer.addSound("Sounds/Piano/piano-a3.wav", (double)delayMillis / 800, 0, 0.35);
    mixer.addSound("Sounds/Piano/piano-c4.wav", (double)delayMillis / 800, 0, 0.35);

    mixer.addSound("Sounds/Piano/piano-b5.wav", (double)delayMillis / 4000, 0, (double)delayMillis / 8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis / 8));
    mixer.addSound("Sounds/Piano/piano-a5.wav", (double)delayMillis / 4000, 0, (double)delayMillis / 8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis / 8));
    mixer.addSound("Sounds/Piano/piano-g5.wav", (double)delayMillis / 4000, 0, (double)delayMillis / 8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis / 8));
    mixer.addSound("Sounds/Piano/piano-f5.wav", (double)delayMillis / 4000, 0, (double)delayMillis / 8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis / 8));
    mixer.addSound("Sounds/Piano/piano-e5.wav", (double)delayMillis / 4000, 0, (double)delayMillis / 8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis / 8));
    mixer.addSound("Sounds/Piano/piano-d5.wav", (double)delayMillis / 4000, 0, (double)delayMillis / 8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis / 8));
    mixer.addSound("Sounds/Piano/piano-c5.wav", (double)delayMillis / 4000, 0, (double)delayMillis / 8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis / 8));
    mixer.addSound("Sounds/Piano/piano-e5.wav", (double)delayMillis / 8000, 0, (double)delayMillis / 8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis / 16));
    mixer.addSound("Sounds/Piano/piano-d5.wav", (double)delayMillis / 8000, 0, (double)delayMillis / 8000, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis / 16));

    mixer.addSound("Sounds/Piano/piano-c4.wav", 3, 0, 0.35);
    mixer.addSound("Sounds/Piano/piano-e4.wav", 3, 0, 0.35);
    mixer.addSound("Sounds/Piano/piano-g4.wav", 3, 0, 0.35);

    mixer.addSound("Sounds/Piano/piano-c5.wav", 3, 0, 0.35, 0.5);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis * 2));
}*/

//CMaj C G swap
/* {
    auto& newSound = mixer.addSound(sound, 4, 0, 0.75);
    newSound.setVolume(.5f);

    auto& newSound2 = mixer.addSound("Sounds/Piano/g3.wav", 4, 0, 0.75f);
    newSound2.setVolume(.5f);

    auto& newSound3 = mixer.addSound("Sounds/Piano/e3.wav", 4, 0, 0.75f, 0.025f);
    newSound3.setVolume(.025f);
    //newSound.setPosition(vec);

    for (float i = -3; i < 3; i += 0.05f)
    {
        vec = { i * i * i,1.0f,0.0f };
        vec2 = { -i * -i * -i,1.0f,0.0f };
        newSound.setPosition(vec);
        newSound2.setPosition(vec2);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    //std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}   */
