#include <iostream>
#include <fmod.hpp>
#include <chrono>
#include <thread>
#include "Music/MusicUtilities.h"
#include "HeaderFiles/SoundMixer.h"

#pragma warning(push, 0)
#include <SFML/Graphics/Glyph.hpp>
#pragma warning(pop)
#pragma warning(disable : 26812)


#include "SFML/Graphics.hpp"
#include "Graphics/GUI/RenderObject.h"
#include "Graphics/GUI/SFButton.h"
#include "Graphics/GUI/RectangleButton.h"
#include "Graphics/GUI/GUIPanel.h"
#include "Graphics/GUI/TextBox.h"
#include "Graphics/GUI/FloatSlider.h"
#include "Graphics/GUI/BoundedFloatSlider.h"
#include "Graphics/GUI/CircleRingSelect.h"
#include "Graphics/GUI/ToggleBox.h"
#include "Graphics/GUI/Primitives/Rectangle.h"
#include "Graphics/GUI/Primitives/PrimitiveText.h"

#include "Graphics/SheetMusic/SheetMusicElement.h"
#include "Graphics/SheetMusic/SheetMusicStaff.h"
#include "Graphics/SheetMusic/SheetMusicClef.h"
#include "Graphics/SheetMusic/SheetMusicMeasure.h"

void checkFmodResult(FMOD_RESULT result) {
    if (result != FMOD_OK) {
        std::cerr << "FMOD error: " << result << std::endl;
        exit(-1);
    }
}

int main() {
    FMOD_RESULT result;
    FMOD::System* system = NULL;
    result = FMOD::System_Create(&system);
    checkFmodResult(result);
    result = system->init(512, FMOD_INIT_NORMAL, nullptr);
    checkFmodResult(result);

    FMOD::Sound* sound = MusicUtilities::getSound("Sounds/Piano/c3.wav", system);
    FMOD_VECTOR listenerPos = { 0.0f, 0.0f, 0.0f };
    FMOD_VECTOR forward = { 0.0f, 1.0f, 0.0f };
    FMOD_VECTOR up = { 0.0f, 0.0f, 1.0f };
    FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };

    result = system->set3DListenerAttributes(0, &listenerPos, &velocity, &forward, &up);
    checkFmodResult(result);

    result = system->createSound("Sounds/Piano/c3.wav", FMOD_3D, nullptr, &sound);
    checkFmodResult(result);

    FMOD_VECTOR soundPosition = { 10.0f, 0.0f, 0.0f }; // 10 units to the left

    //result = system->playSound(sound, nullptr, true, &channel);
    //checkFmodResult(result);
    //result = channel->set3DAttributes(&soundPosition, nullptr);
    //checkFmodResult(result);
    //result = channel->setPaused(false);
    //checkFmodResult(result);

    //// Main loop (simulate for a short period to allow sound to play)
    //for (int i = 0; i < 100; ++i) {
    //    system->update();
    //    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    //}

    //result = sound->release();
    //checkFmodResult(result);

    SoundMixer& mixer = SoundMixer::getInstance();

    mixer.start();
    mixer.setSystem(system);

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
    float bpm = 1;
    int delayMillis = (int)((bpm / 60) * 1000);
    //A more Complex Pattern
    /*{
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
    FMOD_VECTOR vec = { 0.0f,1.0f,0.0f };
    FMOD_VECTOR vec2 = { 0.0f,1.0f,0.0f };

    //CMaj C G swap
    {
        auto& newSound = mixer.addSound(sound, 4, 0, 0.75);
        newSound.setVolume(.5);

        auto& newSound2 = mixer.addSound("Sounds/Piano/g3.wav", 4, 0, 0.75);
        newSound2.setVolume(.025);

        auto& newSound3 = mixer.addSound("Sounds/Piano/e3.wav", 4, 0, 0.75, 0.025);
        newSound3.setVolume(.025);
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
    }   

    sf::RenderWindow window(sf::VideoMode(1200, 800), "Sheet Music Generator");
    sf::Event e;

    //adding objects
    std::vector<RenderObject*> renderObjects;

    auto button =  new PrimitiveText(100.0f,300.0f,24,"This is a test", "resources/fonts/Century 751 Bold.otf", ALIGN_LEFT);
    button->setColor(sf::Color(20, 20, 60));
    renderObjects.push_back(button);


    for (auto& obj : renderObjects) {
        obj->setWindow(&window);
    }

    Pitch note = { NoteC, 3 };

    // chords are vectors of notes
    auto chordTest = MusicUtilities::getNotes(note, MAJOR);
    auto chordTest2 = MusicUtilities::getNotes(note, MINOR);

    sf::Texture texture;
    if (!texture.loadFromFile("resources/images/test.png")) {
        // Handle error
        return EXIT_FAILURE;
    }

    std::vector<std::unique_ptr<sf::Drawable>> drawables;


    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(100, 100);
    sprite.setColor(sf::Color(255, 0, 0, 255));
    
    float sHeight = 200;
    float newScale = sHeight / sprite.getLocalBounds().height;

    sprite.setScale(newScale, newScale);


    std::vector<std::unique_ptr<SheetMusicElement>> sheetMusicObjects;

    //Creation of the staff
    auto staff = new SheetMusicStaff((float)100, (float)400, (float)1000, (float)60, 
        TrebleClef, MusicUtilities::getKey(NoteFs, MAJOR));

    staff->setColor(sf::Color(94, 94, 255));
    staff->setClefColor(sf::Color(150, 150, 255));
    staff->setNoteColor(sf::Color(94, 150, 255));
    staff->setNoteHoverColor(sf::Color(55, 220, 255));
    staff->setHoverColor(sf::Color(255, 200, 255));
    staff->setWindow(&window);

    sf::RectangleShape rect(sf::Vector2f(100, 100));
    rect.setFillColor(sf::Color(255, 0, 0));
    

    rect.setOrigin(sf::Vector2f(25, 25));
    

    Note E3Note = Note({ Pitch({NoteE, 5 }), Quarter });
    Note C3Note = Note({ Pitch({NoteC, 5 }), Quarter });
    Note G3Note = Note({ Pitch({NoteG, 5 }), Quarter });
    Note F5Note = Note({ Pitch({NoteB, 5 }), Quarter });
    Note D3Note = Note({ Pitch({NoteD, 5}), Sixteenth });

    float currentBeat = 1.0f;

    auto key = MusicUtilities::getKey(NoteEf, MAJOR);
    auto distance = MusicUtilities::getNotesFromMiddleC(Pitch({ NoteD,  3 }));
    distance = MusicUtilities::getNotesFromMiddleC(Pitch({ NoteFs,  8 }));

    //Adding Notes
    currentBeat = 0.0f;
    
    staff->addNote(C3Note, currentBeat);
    staff->addNote(E3Note, currentBeat);
    staff->addNote(G3Note, currentBeat);
    currentBeat = staff->addNote(F5Note, currentBeat);
    staff->addNote(D3Note, currentBeat);
    staff->addNote(D3Note, 8.0f);
    staff->addNote(D3Note, 8.5f);
    staff->addNote(D3Note, 12.5f);
    staff->addNote(D3Note, 13.0f);

    staff->colorUpdate();
    renderObjects.push_back(staff);

    auto scale = Scale(Pitch({ NoteEf, 3 }), MAJOR);

    auto currentFrame = std::chrono::high_resolution_clock::now();
    auto previousFrame = std::chrono::high_resolution_clock::now();
    int temp_i = 0;
    auto microsecondCount = 0.0f;

    PrimitiveText fpsText((float)window.getSize().x - 20, (float)window.getSize().y - 30, 24, "FPS: 0", "resources/fonts/Century 751 Bold.otf", ALIGN_RIGHT);
    fpsText.setColor(sf::Color(60, 60, 180));
    fpsText.setWindow(&window);

    TextBox<std::string>* textBox = new TextBox<std::string>(100, 100, 150, 20, 14, "", sf::Color(11,0,44), sf::Color(76, 62, 196), sf::Color(5, 0, 20), sf::Color(94, 150, 255));
    textBox->setWindow(&window);

    renderObjects.push_back(textBox);

    auto floatBox = new TextBox<float>(100, 130, 150, 20, 14, "", sf::Color(11, 0, 44), sf::Color(76, 62, 196), sf::Color(5, 0, 20), sf::Color(94, 150, 255));
    floatBox->setWindow(&window);

    renderObjects.push_back(floatBox);

    floatBox->setValue(420.69f);

    auto intBox = new TextBox<int>(100, 160, 150, 20, 14, "", sf::Color(11, 0, 44), sf::Color(76, 62, 196), sf::Color(5, 0, 20), sf::Color(94, 150, 255));
    intBox->setWindow(&window);

    renderObjects.push_back(intBox);

    intBox->setValue(5);

    textBox = new TextBox<std::string>(100, 190, 150, 20, 14, "", sf::Color(11, 0, 44), sf::Color(76, 62, 196), sf::Color(5, 0, 20), sf::Color(94, 150, 255));
    textBox->setWindow(&window);

    renderObjects.push_back(textBox);

    textBox = new TextBox<std::string>(100, 190, 150, 20, 14, "", sf::Color(11, 0, 44), sf::Color(76, 62, 196), sf::Color(5, 0, 20), sf::Color(94, 150, 255));
    textBox->setWindow(&window);

    renderObjects.push_back(textBox);

    textBox->setValue("This is the string now");

    BoundedFloatSlider* boundedFloatSlider = new BoundedFloatSlider(100, 220, 150, 20, 
        sf::Color(11, 0, 44), sf::Color(76, 62, 196), sf::Color(76, 62, 196), sf::Color(247, 235, 236), sf::Color(5, 0, 20), sf::Color(96, 82, 216), sf::Color(94, 150, 255));
    boundedFloatSlider->setWindow(&window);
    boundedFloatSlider->setBounds(0, 360);

    renderObjects.push_back(boundedFloatSlider);

    FloatSlider* floatSlider = new FloatSlider(100, 250, 150, 20,
        sf::Color(11, 0, 44), sf::Color(76, 62, 196), sf::Color(247, 235, 236), sf::Color(5, 0, 20), sf::Color(94, 150, 255));
    floatSlider->setWindow(&window);

    renderObjects.push_back(floatSlider);

    CircleRingSelect* circleSelection = new CircleRingSelect(400, 200, 50, 10, 
        sf::Color(11, 0, 44), sf::Color(11, 0, 44), sf::Color(76, 62, 196), sf::Color(76, 62, 196), sf::Color(5, 0, 20), sf::Color(94, 150, 255));
    circleSelection->setWindow(&window);
    circleSelection->setSlider(boundedFloatSlider);

    renderObjects.push_back(circleSelection);


    ToggleBox* toggle = new ToggleBox(500, 150, 20, 20, 
        sf::Color(11, 0, 44), sf::Color(76, 62, 196), sf::Color(76, 62, 196), sf::Color(5, 0, 20), sf::Color(94, 150, 255));
    toggle->setWindow(&window);

    renderObjects.push_back(toggle);

    RenderObject* hoverObject = nullptr;
    RenderObject* previousHoverObject = nullptr;
    RenderObject* activeObject = nullptr;

    std::string inputString;

    auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();

    int lastNote = 0;

    std::map<int, FMOD::Sound*> soundMap;

    soundMap[0] = MusicUtilities::getSound("Sounds/Piano/c3.wav", system);
    soundMap[1] = MusicUtilities::getSound("Sounds/Piano/d3.wav", system);
    soundMap[2] = MusicUtilities::getSound("Sounds/Piano/e3.wav", system);
    soundMap[3] = MusicUtilities::getSound("Sounds/Piano/f3.wav", system);
    soundMap[4] = MusicUtilities::getSound("Sounds/Piano/g3.wav", system);
    soundMap[5] = MusicUtilities::getSound("Sounds/Piano/a3.wav", system);
    soundMap[6] = MusicUtilities::getSound("Sounds/Piano/b3.wav", system);
    soundMap[7] = MusicUtilities::getSound("Sounds/Piano/c4.wav", system);
    
    while (window.isOpen())
    {
        //Frame data
        
        temp_i++;

        currentFrame = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(currentFrame - previousFrame);

        previousFrame = std::chrono::high_resolution_clock::now();

        microsecondCount += duration.count();

        if (temp_i % 200 == 0)
        {
            auto averageDuration = microsecondCount / 200.0f;
            //std::cout << "Average of 100 frames: " << averageDuration << " microseconds\n";
            microsecondCount = 0;
            auto fps = 1000000.0f / averageDuration;
            std::string fpsString = "FPS: " + std::to_string((int)-fps);
            fpsText.setText(fpsString);
        }

        //Active and Hover object managment
        hoverObject = nullptr;
        for (auto& obj : renderObjects) {
            auto& object = obj->getHoverObject();
            obj->setHover(false);

            if (object.getType() != EmptyRenderObject)
            {
                object.setHover(true);
                hoverObject = &object;
            }
        }

        if (hoverObject != previousHoverObject && previousHoverObject != nullptr)
        {
            if (activeObject != previousHoverObject)
            {
                previousHoverObject->setUnhover();
            }
        }
        previousHoverObject = hoverObject;

        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                //stop the mixer thread 
                // WILL RESULT IN ERROR IF LEFT RUNNNING
                mixer.stop();
                window.close();
            }
            if (e.type == sf::Event::MouseButtonPressed) {
                if (e.mouseButton.button == sf::Mouse::Left) {
                    auto& newSound = mixer.addSound(soundMap[0], 4, 0, 0.75);
                    newSound.setVolume(0.25f);
                    // Handle left mouse button pressed
                    int mouseX = e.mouseButton.x;
                    int mouseY = e.mouseButton.y;

                    if (activeObject != nullptr && hoverObject != activeObject)
                    {
                        activeObject->setInactive();
                        activeObject = nullptr;
                    }
                    if (hoverObject != nullptr)
                    {
                        hoverObject->onClick();
                        activeObject = hoverObject;
                        activeObject->setActive();
                    }
                }
            }

            if (e.type == sf::Event::TextEntered) {
                if (e.text.unicode <= 128) { // Basic ASCII filter
                    if (activeObject != nullptr)
                    {
                        activeObject->keyboardInput(e.text.unicode);
                    }
                }
            }

            if (e.type == sf::Event::KeyPressed) {
                
                switch (e.key.code)
                {
                case sf::Keyboard::Home:
                case sf::Keyboard::End:
                case sf::Keyboard::Delete:
                case sf::Keyboard::Up:
                case sf::Keyboard::Down:
                case sf::Keyboard::Left:
                case sf::Keyboard::Right:
                    if (activeObject != nullptr)
                        activeObject->arrowKeyInput(e.key.code);
                    break;
                default:
                    break;
                }
            }
        }

        // Clear screen
        window.clear(sf::Color(0,3,25));

        fpsText.draw();

        for (auto& obj : renderObjects) {
            obj->render();
        }
        if (hoverObject != nullptr)
        {
            hoverObject->hoverDraw();
        }
        if (activeObject != nullptr)
        {
            /*if (activeObject->getActiveOverride())
            {
                activeObject = &(activeObject->getHoverObject());
            }*/
            if (!activeObject->getActive())
            {
                activeObject->setInactive();
                activeObject = nullptr;
            }
            else
            {
                activeObject->activeDraw();
            }
        }


        //Sound shit
        if (toggle->getState())
        {
            currentTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

            int playNote = (duration.count() / 1000) % 8;
            if (playNote != lastNote)
            {
                lastNote = playNote;
                std::cout << playNote << std::endl;
                
            }

        }


        // Update the window
        window.display();
    }

    return 0;
}