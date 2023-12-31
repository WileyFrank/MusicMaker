#include <iostream>
#include <fmod.hpp>
#include <chrono>
#include <thread>
#include "Music/MusicUtilities.h"
#include "HeaderFiles/SoundMixer.h"

#pragma warning(push, 0)
#include <SFML/Graphics/Glyph.hpp>
#pragma warning(pop)

#include "SFML/Graphics.hpp"
#include "Graphics/GUI/RenderObject.h"
#include "Graphics/GUI/SFButton.h"
#include "Graphics/GUI/RectangleButton.h"
#include "Graphics/GUI/GUIPanel.h"
#include "Graphics/GUI/Primitives/Rectangle.h"
#include "Graphics/GUI/Primitives/PrimitiveText.h"

#include "Graphics/SheetMusic/SheetMusicElement.h"
#include "Graphics/SheetMusic/SheetMusicStaff.h"
#include "Graphics/SheetMusic/SheetMusicClef.h"
#include "Graphics/SheetMusic/SheetMusicMeasure.h"



int main() {
    FMOD::System* system = NULL;
    FMOD::System_Create(&system);
    system->init(513, FMOD_INIT_NORMAL, 0);

    FMOD_VECTOR listenerPos = { 0.0f, 0.0f, 0.0f };
    FMOD_VECTOR forward = { 0.0f, 1.0f, 0.0f };
    FMOD_VECTOR up = { 0.0f, 0.0f, 1.0f };

    system->set3DListenerAttributes(0, &listenerPos, NULL, &forward, &up);

    SoundMixer& mixer = SoundMixer::getInstance();

    mixer.start();
    mixer.setSystem(system);
    
    //Cmaj I IV V I
    {

        /*
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

        */
    }
    float bpm = 1;
    int delayMillis = (int)((bpm / 60) * 1000);
    //A more Complex Pattern
    {
        /*std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis / 8));

        
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
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMillis * 2));*/
    }

    system->close();
    system->release();


    sf::RenderWindow window(sf::VideoMode(800, 800), "Test");
    sf::Event e;

    //adding objects
    std::vector<std::unique_ptr<RenderObject>> renderObjects;

    auto button = std::make_unique<PrimitiveText>(100.0f,300.0f,24,"This is a test", "resources/fonts/Century 751 Bold.otf", ALIGN_LEFT);
    button->setColor(sf::Color(20, 20, 60));
    renderObjects.push_back(std::unique_ptr<RenderObject>(std::move(button)));


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
    auto staff = std::make_unique<SheetMusicStaff>((float)100, (float)400, (float)500, (float)50, 
        TrebleClef, MusicUtilities::getKey(NoteG, MAJOR));

    staff->setColor(sf::Color(94, 94, 255));
    staff->setClefColor(sf::Color(150, 150, 255));
    staff->setNoteColor(sf::Color(94, 150, 255));
    staff->setNoteHoverColor(sf::Color(55, 220, 255));
    staff->setHoverColor(sf::Color(255, 150, 255));
    staff->setWindow(&window);

    sf::RectangleShape rect(sf::Vector2f(100, 100));
    rect.setFillColor(sf::Color(255, 0, 0));
    

    rect.setOrigin(sf::Vector2f(25, 25));
    

    Note C3Note = Note({ Pitch({NoteC, 5 }), Eighth });
    Note E3Note = Note({ Pitch({NoteE, 5 }), Eighth });
    Note G3Note = Note({ Pitch({NoteG, 5 }), Eighth });
    Note F5Note = Note({ Pitch({NoteFs, 5 }), Eighth });
    Note D3Note = Note({ Pitch({NoteD, 5}), Sixteenth });

    float currentBeat = 1.0f;

    auto key = MusicUtilities::getKey(NoteEf, MAJOR);
    auto distance = MusicUtilities::getNotesFromMiddleC(Pitch({ NoteD,  3 }));
    distance = MusicUtilities::getNotesFromMiddleC(Pitch({ NoteFs,  8 }));


    auto restTest = staff->addMeasure();

    currentBeat = 0.0f;
    
    
    restTest->addNote(C3Note, currentBeat);
    restTest->addNote(E3Note, currentBeat);

    currentBeat = restTest->addNote(G3Note, currentBeat);
    currentBeat = restTest->addNote(D3Note, currentBeat);
    //currentBeat = restTest->addNote(F5Note, currentBeat);
    //currentBeat = restTest->addNote(F5Note, currentBeat);
    // 
    //currentBeat = restTest->addNote(D3Note, currentBeat);

    ////currentBeat = measureTest.addNote(D3Note, currentBeat);
    //currentBeat = restTest->addNote(C3Note, currentBeat + 0.5f);
    //currentBeat = restTest->addNote(D3Note, currentBeat);
    //currentBeat = restTest->addNote(D3Note, currentBeat);

    restTest->addRests();
    restTest->reload();

    auto smkey = staff->getSheetMusicKeySignature();
    auto smclef = staff->getSheetMusicClef();


    staff->colorUpdate();
    renderObjects.push_back(std::move(staff));




    auto scale = Scale(Pitch({ NoteEf, 3 }), MAJOR);

    //Add scales and notes quickly to measues, and staffs, in a way that can be later automated

    auto acc = MusicUtilities::getAccidentalsInKey(C3Note, key);

    /*auto renderRect = std::make_unique<GUIPanel>(20.0f, 20.0f, 50.0f,50.0f,sf::Color(255,0,255),sf::Color(100,100,255), 2.0f);
    renderRect->setWindow(&window);
    renderObjects.push_back(std::move(renderRect));*/

    auto currentFrame = std::chrono::high_resolution_clock::now();
    auto previousFrame = std::chrono::high_resolution_clock::now();
    int temp_i = 0;
    auto microsecondCount = 0;


    auto intervals = MusicUtilities::generateIntervals({ {NoteA, 3},{NoteCs, 4}, {NoteE, 4} });


    while (window.isOpen())
    {
        //Frame data
        /*
        temp_i++;

        currentFrame = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(currentFrame - previousFrame);

        previousFrame = std::chrono::high_resolution_clock::now();

        microsecondCount += duration.count();

        if (temp_i % 1000 == 0)
        {
            auto averageDuration = microsecondCount / 1000.0f;
            std::cout << "Average of 500 frames: " << averageDuration << " microseconds\n";
            microsecondCount = 0;
        }
        */



        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                //stop the mixer thread 
                // WILL RESULT IN ERROR IF LEFT RUNNNING
                mixer.stop();
                window.close();
            }
        }

        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

        //std::cout << "x: " << mousePosition.x << "\t\ty: " << mousePosition.y << "\n";
        rect.setPosition(sf::Vector2f((float)mousePosition.x, (float)mousePosition.y));


        // Clear screen
        window.clear(sf::Color(0,3,25));

        //smclef->drawBoundingBox();

        //Draw all objects
        for (auto& obj : renderObjects) {
            obj->render(); // This will call the render method of the actual object type
            auto& object = obj->getHoverObject();
            obj->setHover(false);

            if (object.getType() != EmptyRenderObject)
            {
                object.setHover(true);
            }
        }

        /*for (auto& sme : sheetMusicObjects)
        {
            sme->draw(); 
        }*/


        //window.draw(rect);
        // Draw the sprite
        /*window.draw(sprite);
        window.draw(line);*/

        // Update the window
        window.display();
    }

    return 0;
}