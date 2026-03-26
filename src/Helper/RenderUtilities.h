#pragma once
#include <fmod.hpp>
#include "SFML/Graphics.hpp"
#include "../Graphics/SheetMusic/SheetMusicStaff.h"
#include "../Graphics/GameObjects/PlayerObject.h"
#include "../Graphics/GUI/RenderObject.h"
#include "../Graphics/GUI/TextBox.h"
#include "../Graphics/GUI/FloatSlider.h"
#include "../Graphics/GUI/BoundedFloatSlider.h"
#include "../Graphics/GUI/CircleRingSelect.h"
#include "../Graphics/GUI/ToggleBox.h"
#include "../Graphics/GUI/DropdownMenu.h"
#include "../Graphics/GUI/GUIPanel.h"
#include "../Graphics/GUI/Theme.h"
#include "../Graphics/GUI/Primitives/PrimitiveText.h"
#include "../HeaderFiles/SoundMixer.h"


class RenderUtilities
{
public:
    static void pollEvents(sf::RenderWindow* guiWindow, sf::RenderWindow* gameWindow, PlayerObject*& player, RenderObject*& activeObject, RenderObject*& hoverObject, bool* layoutDirty = nullptr)
    {

        sf::Event e;
        while (guiWindow->pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                //stop the mixer thread 
                // WILL RESULT IN ERROR IF LEFT RUNNNING
                guiWindow->close();
                // Ensure the app exits even though the game window is hidden.
                gameWindow->close();
            }
            if (e.type == sf::Event::Resized)
            {
                sf::View resizedView(sf::FloatRect(
                    0.0f,
                    0.0f,
                    static_cast<float>(e.size.width),
                    static_cast<float>(e.size.height)
                ));
                guiWindow->setView(resizedView);

                if (layoutDirty != nullptr)
                {
                    *layoutDirty = true;
                }
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

    static void resolveLayoutTree(std::vector<RenderObject*>& renderObjects, const sf::FloatRect& rootRect)
    {
        for (auto* object : renderObjects)
        {
            if (object != nullptr)
            {
                object->resolveLayout(rootRect);
            }
        }
    }

    static void updateUiLayout(std::vector<RenderObject*>& renderObjects, sf::RenderWindow* guiWindow)
    {
        if (guiWindow == nullptr)
        {
            return;
        }

        const sf::Vector2u windowSize = guiWindow->getSize();
        const sf::FloatRect rootRect(
            0.0f,
            0.0f,
            static_cast<float>(windowSize.x),
            static_cast<float>(windowSize.y)
        );
        resolveLayoutTree(renderObjects, rootRect);
    }

    static void initializeRenderObjects(
        std::vector<RenderObject*>& renderObjects, sf::RenderWindow* guiWindow, sf::RenderWindow* gameWindow,
        ToggleBox*& toggle, CircleRingSelect*& circleSelection, FloatSlider*& floatSlider
    )
    {
        (void)gameWindow;

        auto button = new PrimitiveText(100.0f, 300.0f, 24, "This is a test", "resources/fonts/Century 751 Bold.otf", ALIGN_LEFT);
        button->setColor(sf::Color(20, 20, 60));
        button->setWindow(guiWindow);
        // renderObjects.push_back(button);

        Pitch note = { NoteC, 3 };

        // chords are vectors of notes
        auto chordTest = MusicUtilities::getNotes(note, MAJOR);
        auto chordTest2 = MusicUtilities::getNotes(note, MINOR);
        (void)chordTest;
        (void)chordTest2;

        sf::Texture texture;
        if (!texture.loadFromFile("resources/images/test.png")) {
            // Handle error
            return;
        }

        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setPosition(100, 100);
        sprite.setColor(sf::Color(255, 0, 0, 255));

        float sHeight = 200;
        float newScale = sHeight / sprite.getLocalBounds().height;
        sprite.setScale(newScale, newScale);

        std::vector<std::unique_ptr<SheetMusicElement>> sheetMusicObjects;
        (void)sheetMusicObjects;

        

        auto scale = Scale(Pitch({ NoteEf, 3 }), MAJOR);
        (void)scale;

        TextBox<std::string>* textBox = new TextBox<std::string>(100, 100, 150, 20, 14, "", sf::Color(11, 0, 44), sf::Color(76, 62, 196), sf::Color(5, 0, 20), sf::Color(94, 150, 255));
        textBox->setWindow(guiWindow);
        // renderObjects.push_back(textBox);

        auto floatBox = new TextBox<float>(100, 130, 150, 20, 14, "", sf::Color(11, 0, 44), sf::Color(76, 62, 196), sf::Color(5, 0, 20), sf::Color(94, 150, 255));
        floatBox->setWindow(guiWindow);
        floatBox->setValue(420.69f);
        // renderObjects.push_back(floatBox);

        auto intBox = new TextBox<int>(100, 160, 150, 20, 14, "", sf::Color(11, 0, 44), sf::Color(76, 62, 196), sf::Color(5, 0, 20), sf::Color(94, 150, 255));
        intBox->setWindow(guiWindow);
        intBox->setValue(5);
        // renderObjects.push_back(intBox);

        textBox = new TextBox<std::string>(100, 190, 150, 20, 14, "", sf::Color(11, 0, 44), sf::Color(76, 62, 196), sf::Color(5, 0, 20), sf::Color(94, 150, 255));
        textBox->setWindow(guiWindow);
        // renderObjects.push_back(textBox);

        textBox = new TextBox<std::string>(100, 190, 150, 20, 14, "", sf::Color(11, 0, 44), sf::Color(76, 62, 196), sf::Color(5, 0, 20), sf::Color(94, 150, 255));
        textBox->setWindow(guiWindow);
        textBox->setValue("This is the string now");
        // renderObjects.push_back(textBox);

        BoundedFloatSlider* boundedFloatSlider = new BoundedFloatSlider(100, 220, 150, 20,
            sf::Color(11, 0, 44), sf::Color(76, 62, 196), sf::Color(76, 62, 196), sf::Color(247, 235, 236), sf::Color(5, 0, 20), sf::Color(96, 82, 216), sf::Color(94, 150, 255));
        boundedFloatSlider->setWindow(guiWindow);
        boundedFloatSlider->setBounds(0, 360);
        // renderObjects.push_back(boundedFloatSlider);

        floatSlider = new FloatSlider(100, 250, 150, 20,
            sf::Color(11, 0, 44), sf::Color(76, 62, 196), sf::Color(247, 235, 236), sf::Color(5, 0, 20), sf::Color(94, 150, 255));
        floatSlider->setWindow(guiWindow);
        floatSlider->setValue(5.0f);
        float slideVal = floatSlider->getValue();
        (void)slideVal;
        // renderObjects.push_back(floatSlider);

        circleSelection = new CircleRingSelect(400, 200, 50, 10,
            sf::Color(11, 0, 44), sf::Color(11, 0, 44), sf::Color(76, 62, 196), sf::Color(76, 62, 196), sf::Color(5, 0, 20), sf::Color(94, 150, 255));
        circleSelection->setWindow(guiWindow);
        circleSelection->setSlider(boundedFloatSlider);
        // renderObjects.push_back(circleSelection);

        auto basePanel = new GUIPanel(
            RectSpec{ Px(0),Px(0),Pct(100),Pct(1.00f) },
            Theme::Panel,
            Theme::BorderPanel,
            2.0f,
            0.0f
        );
        basePanel->setWindow(guiWindow);
        basePanel->setPadding(5.0f);


        auto topPanel = new GUIPanel(
            RectSpec{ Px(0),Px(0),Pct(100),Pct(50) },
            Theme::PanelAlt,
            Theme::BorderSubtle,
            2.0f,
            5.0f
        );
        basePanel->addChild(topPanel);

        auto bottomPanel = new GUIPanel(
            RectSpec{ Px(0),Pct(50),Pct(100),Pct(50) },
            Theme::PanelAlt,
            Theme::BorderSubtle,
            2.0f,
            0.0f
        );
        basePanel->addChild(bottomPanel);



        toggle = new ToggleBox(
            RectSpec{ Pct(0.05f), Pct(0.05f), Px(20), Px(20) },
            MarginSpec{ Px(0), Px(0), Px(0), Px(0) },
            Theme::Control, Theme::Accent, Theme::Accent, Theme::ControlHover, Theme::AccentBright);
        topPanel->addChild(toggle);

        // Creation of the staff

        auto staff = new SheetMusicStaff(RectSpec{Pct(5),Pct(20),Pct(90),Px(50)}, MarginSpec{Px(0), Px(0), Px(0), Px(0)},
            TrebleClef, MusicUtilities::getKey(NoteG, MAJOR));

        staff->setColor(Theme::StaffLine);
        staff->setClefColor(Theme::Clef);
        staff->setNoteColor(Theme::Note);
        staff->setNoteHoverColor(Theme::NoteHover);
        staff->setHoverColor(Theme::StaffHover);

        sf::RectangleShape rect(sf::Vector2f(100, 100));
        rect.setFillColor(sf::Color(255, 0, 0));
        rect.setOrigin(sf::Vector2f(25, 25));
        (void)rect;

        Note E3Note = Note({ Pitch({NoteE, 5 }), Quarter });
        Note C3Note = Note({ Pitch({NoteC, 5 }), Quarter });
        Note G3Note = Note({ Pitch({NoteG, 5 }), Quarter });
        Note F5Note = Note({ Pitch({NoteB, 5 }), Quarter });
        Note D3Note = Note({ Pitch({NoteD, 5}), Sixteenth });

        float currentBeat = 1.0f;

        auto key = MusicUtilities::getKey(NoteEf, MAJOR);
        auto distance = MusicUtilities::getNotesFromMiddleC(Pitch({ NoteD,  3 }));
        distance = MusicUtilities::getNotesFromMiddleC(Pitch({ NoteFs,  8 }));
        (void)key;
        (void)distance;

        // Adding Notes
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
        staff->addNote(D3Note, 17.0f);
        staff->addNote(D3Note, 14.0f);
        bottomPanel->addChild(staff);



        renderObjects.push_back(basePanel);

        std::vector<std::string> options;
        options.push_back("This");
        options.push_back("That");
        options.push_back("This One");
        options.push_back("That One");
        options.push_back("This One Too");

        auto dropdown = new DropdownMenu<std::string>(
            RectSpec{ Pct(0.5f), Pct(0.35f), Pct(0.125f), Pct(0.05f) },
            MarginSpec{ Px(0), Px(0), Px(0), Px(0) },
            14, 5, options,
            Theme::Control, Theme::Accent, Theme::ControlHover, Theme::AccentBright);
        topPanel->addChild(dropdown);
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
                previousHoverObject->setHover(false);
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
        }

        auto currentTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

        int playNote = (duration.count() / 500) % 8;
        if (playNote != lastNote)
        {
            lastNote = playNote;
            std::cout << playNote << std::endl;
            auto handle = mixer.playSound(soundMap[playNote], 1, 0.0625, 0.25);
            mixer.setSoundVolume(handle, 0.25f);

            vec = { -std::cos(positionAngle) * positionDistance, -std::sin(positionAngle) * positionDistance, 0 };

            mixer.setSoundPosition(handle, vec);
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

