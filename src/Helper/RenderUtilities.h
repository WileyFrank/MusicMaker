#pragma once
#include <fmod.hpp>
#include "SFML/Graphics.hpp"
#include "../Graphics/SheetMusic/SheetMusicStaff.h"
#include "../Graphics/GUI/RenderObject.h"


class RenderUtilities
{
public:
    static void pollEvents(sf::RenderWindow* guiWindow, sf::RenderWindow* gameWindow, PlayerObject*& player, RenderObject*& activeObject, RenderObject*& hoverObject)
    {

        sf::Event e;
        while (guiWindow->pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                //stop the mixer thread 
                // WILL RESULT IN ERROR IF LEFT RUNNNING
                guiWindow->close();
            }
            if (e.type == sf::Event::MouseButtonPressed) {
                if (e.mouseButton.button == sf::Mouse::Left) {

                    // Handle left mouse button pressed
                    int mouseX = e.mouseButton.x;
                    int mouseY = e.mouseButton.y;

                    if (activeObject != nullptr && hoverObject != activeObject)
                    {
                        (activeObject)->setInactive();
                        (activeObject) = nullptr;
                    }
                    if (hoverObject != nullptr)
                    {
                        activeObject = hoverObject;
                        activeObject->setActive();
                        hoverObject->onClick();
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

        while (gameWindow->pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                gameWindow->close();
            }

            if (e.type == sf::Event::KeyPressed) {
                player->arrowKeyInput(e.key.code);
            }
        }
    }

    static void initializeRenderObjects(
        std::vector<RenderObject*>& renderObjects, sf::RenderWindow* guiWindow, sf::RenderWindow* gameWindow,
        ToggleBox*& toggle, CircleRingSelect*& circleSelection, FloatSlider*& floatSlider
    )
    {

        auto button = new PrimitiveText(100.0f, 300.0f, 24, "This is a test", "resources/fonts/Century 751 Bold.otf", ALIGN_LEFT);
        button->setColor(sf::Color(20, 20, 60));
        renderObjects.push_back(button);


        for (auto& obj : renderObjects) {
            obj->setWindow(guiWindow);
        }

        Pitch note = { NoteC, 3 };

        // chords are vectors of notes
        auto chordTest = MusicUtilities::getNotes(note, MAJOR);
        auto chordTest2 = MusicUtilities::getNotes(note, MINOR);

        sf::Texture texture;
        if (!texture.loadFromFile("resources/images/test.png")) {
            // Handle error
            return;
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
        staff->setWindow(guiWindow);

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

        TextBox<std::string>* textBox = new TextBox<std::string>(100, 100, 150, 20, 14, "", sf::Color(11, 0, 44), sf::Color(76, 62, 196), sf::Color(5, 0, 20), sf::Color(94, 150, 255));
        textBox->setWindow(guiWindow);

        renderObjects.push_back(textBox);

        auto floatBox = new TextBox<float>(100, 130, 150, 20, 14, "", sf::Color(11, 0, 44), sf::Color(76, 62, 196), sf::Color(5, 0, 20), sf::Color(94, 150, 255));
        floatBox->setWindow(guiWindow);

        renderObjects.push_back(floatBox);

        floatBox->setValue(420.69f);

        auto intBox = new TextBox<int>(100, 160, 150, 20, 14, "", sf::Color(11, 0, 44), sf::Color(76, 62, 196), sf::Color(5, 0, 20), sf::Color(94, 150, 255));
        intBox->setWindow(guiWindow);

        renderObjects.push_back(intBox);

        intBox->setValue(5);

        textBox = new TextBox<std::string>(100, 190, 150, 20, 14, "", sf::Color(11, 0, 44), sf::Color(76, 62, 196), sf::Color(5, 0, 20), sf::Color(94, 150, 255));
        textBox->setWindow(guiWindow);

        renderObjects.push_back(textBox);

        textBox = new TextBox<std::string>(100, 190, 150, 20, 14, "", sf::Color(11, 0, 44), sf::Color(76, 62, 196), sf::Color(5, 0, 20), sf::Color(94, 150, 255));
        textBox->setWindow(guiWindow);

        renderObjects.push_back(textBox);

        textBox->setValue("This is the string now");

        BoundedFloatSlider* boundedFloatSlider = new BoundedFloatSlider(100, 220, 150, 20,
            sf::Color(11, 0, 44), sf::Color(76, 62, 196), sf::Color(76, 62, 196), sf::Color(247, 235, 236), sf::Color(5, 0, 20), sf::Color(96, 82, 216), sf::Color(94, 150, 255));
        boundedFloatSlider->setWindow(guiWindow);
        boundedFloatSlider->setBounds(0, 360);

        renderObjects.push_back(boundedFloatSlider);

        floatSlider = new FloatSlider(100, 250, 150, 20,
            sf::Color(11, 0, 44), sf::Color(76, 62, 196), sf::Color(247, 235, 236), sf::Color(5, 0, 20), sf::Color(94, 150, 255));
        floatSlider->setWindow(guiWindow);

        floatSlider->setValue(5.0f);
        float slideVal = floatSlider->getValue();

        renderObjects.push_back(floatSlider);

        circleSelection = new CircleRingSelect(400, 200, 50, 10,
            sf::Color(11, 0, 44), sf::Color(11, 0, 44), sf::Color(76, 62, 196), sf::Color(76, 62, 196), sf::Color(5, 0, 20), sf::Color(94, 150, 255));
        circleSelection->setWindow(guiWindow);
        circleSelection->setSlider(boundedFloatSlider);

        renderObjects.push_back(circleSelection);


        toggle = new ToggleBox(500, 150, 20, 20,
            sf::Color(11, 0, 44), sf::Color(76, 62, 196), sf::Color(76, 62, 196), sf::Color(5, 0, 20), sf::Color(94, 150, 255));
        toggle->setWindow(guiWindow);

        renderObjects.push_back(toggle);

        std::vector<std::string> options;
        options.push_back("This");
        options.push_back("That");

        auto dropdown = new DropdownMenu<std::string>(600, 150, 150, 20, 14, 5, options,
            sf::Color(11, 0, 44), sf::Color(76, 62, 196), sf::Color(5, 0, 20), sf::Color(94, 150, 255));
        dropdown->setWindow(guiWindow);

        renderObjects.push_back(dropdown);
    }

    static void setActiveHover(std::vector<RenderObject*>& renderObjects, RenderObject*& hoverObject, RenderObject*& previousHoverObject, RenderObject*& activeObject)
    {
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
    }

    static void playScale(SoundMixer& mixer, std::map<int, FMOD::Sound*> soundMap,
        float& positionAngle, float& positionDistance, CircleRingSelect*& circleSelection, FloatSlider*& floatSlider,
        int& lastNote, std::chrono::steady_clock::time_point startTime)
    {
        FMOD_VECTOR vec;
        if (positionAngle != circleSelection->getAngle() || positionDistance != floatSlider->getValue())
        {
            positionAngle = circleSelection->getAngle();
            positionDistance = floatSlider->getValue();

            vec = { -std::cos(positionAngle) * positionDistance, -std::sin(positionAngle) * positionDistance, 0 };

            mixer.setPosition(vec);

        }

        auto currentTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

        int playNote = (duration.count() / 500) % 8;
        if (playNote != lastNote)
        {
            lastNote = playNote;
            std::cout << playNote << std::endl;
            auto& newSound = mixer.addSound(soundMap[playNote], 1, 0.0625, 0.25);
            newSound.setVolume(0.25f);

            vec = { -std::cos(positionAngle) * positionDistance, -std::sin(positionAngle) * positionDistance, 0 };

            newSound.setPosition(vec);
        }
    }

    static void drawRenderObjects(std::vector<RenderObject*>& renderObjects, RenderObject*& hoverObject, RenderObject*& activeObject)
    {
        for (auto& obj : renderObjects) {
            obj->render();
        }
        if (hoverObject != nullptr)
        {
            hoverObject->hoverDraw();
        }
        if (activeObject != nullptr)
        {
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
    }
};

