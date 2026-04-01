#pragma once
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <cctype>
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
#include "../Graphics/GUI/RectangleButton.h"
#include "../Graphics/GUI/GUIButton.h"
#include "../Graphics/GUI/ImageButton.h"
#include "../Graphics/GUI/GUIPanel.h"
#include "../Graphics/GUI/Theme.h"
#include "../Graphics/GUI/Primitives/PrimitiveText.h"
#include "../HeaderFiles/SoundMixer.h"


class RenderUtilities
{
public:
    /** Used when clamping GUI window size (SFML 2.x has no setMinimumSize; SFML 3 adds it on Window). */
    static constexpr unsigned kGuiWindowMinWidth = 960;
    static constexpr unsigned kGuiWindowMinHeight = 600;

    /** Reads current checked state of each ToggleBox child (in child order: toggles only). */
    static std::vector<bool> collectChordToggleStates(GUIPanel* chordPanel)
    {
        std::vector<bool> states;
        if (chordPanel == nullptr)
        {
            return states;
        }
        for (auto* child : chordPanel->getChildren())
        {
            if (child == nullptr)
            {
                continue;
            }
            auto* tb = dynamic_cast<ToggleBox*>(child);
            if (tb != nullptr)
            {
                states.push_back(tb->getState());
            }
        }
        return states;
    }

    /**
     * Steps the root note letter (first character only: A–G, suffix left unchanged).
     * @param increment true = A→B→…→G→A, false = reverse.
     */
    static void adjustRootLetter(TextBox<std::string>* rootTextBox, bool increment)
    {
        if (rootTextBox == nullptr)
        {
            return;
        }

        std::string s = rootTextBox->getTextString();
        if (s.empty())
        {
            s = "C";
        }

        char first = static_cast<char>(std::toupper(static_cast<unsigned char>(s[0])));
        if (first < 'A' || first > 'G')
        {
            first = 'C';
        }

        int idx = first - 'A';
        if (increment)
        {
            idx = (idx + 1) % 7;
        }
        else
        {
            idx = (idx + 6) % 7;
        }

        s[0] = static_cast<char>('A' + idx);
        rootTextBox->setValue(std::move(s));
    }

    /**
     * Right-click root steppers: + adds sharp(s), - adds flat(s), up to two of the same kind.
     * If the opposite accidental is present, removes one of that symbol instead of stacking more.
     */
    static void adjustRootAccidental(TextBox<std::string>* rootTextBox, bool addSharp)
    {
        if (rootTextBox == nullptr)
        {
            return;
        }

        std::string s = rootTextBox->getTextString();
        if (s.empty())
        {
            s = "C";
        }

        char letter = static_cast<char>(std::toupper(static_cast<unsigned char>(s[0])));
        if (letter < 'A' || letter > 'G')
        {
            letter = 'C';
        }

        std::string suffix = s.size() > 1 ? s.substr(1) : "";

        if (addSharp)
        {
            const size_t pos = suffix.rfind('b');
            if (pos != std::string::npos)
            {
                suffix.erase(pos, 1);
            }
            else
            {
                int sharpCount = 0;
                for (char c : suffix)
                {
                    if (c == '#')
                    {
                        sharpCount++;
                    }
                }
                if (sharpCount < 2)
                {
                    suffix += '#';
                }
            }
        }
        else
        {
            const size_t pos = suffix.rfind('#');
            if (pos != std::string::npos)
            {
                suffix.erase(pos, 1);
            }
            else
            {
                int flatCount = 0;
                for (char c : suffix)
                {
                    if (c == 'b')
                    {
                        flatCount++;
                    }
                }
                if (flatCount < 2)
                {
                    suffix += 'b';
                }
            }
        }

        std::string out;
        out.push_back(letter);
        out += suffix;
        rootTextBox->setValue(std::move(out));
    }

    /** Valid octave range for the chord octave field (matches TextBox validator). */
    static constexpr int kOctaveMin = 0;
    static constexpr int kOctaveMax = 8;

    /**
     * Steps octave up/down without leaving [kOctaveMin, kOctaveMax].
     * @param increment true = +1, false = -1 (no-op at the corresponding bound).
     */
    static void adjustOctave(TextBox<int>* octaveTextBox, bool increment)
    {
        if (octaveTextBox == nullptr)
        {
            return;
        }

        int current = 4;
        const std::string& t = octaveTextBox->getTextString();
        if (!t.empty())
        {
            try
            {
                current = std::stoi(t);
            }
            catch (...)
            {
                current = octaveTextBox->getValue();
            }
        }
        else
        {
            current = octaveTextBox->getValue();
        }

        if (current < kOctaveMin)
        {
            current = kOctaveMin;
        }
        if (current > kOctaveMax)
        {
            current = kOctaveMax;
        }

        if (increment)
        {
            if (current < kOctaveMax)
            {
                current++;
            }
        }
        else
        {
            if (current > kOctaveMin)
            {
                current--;
            }
        }

        octaveTextBox->setValue(current);
    }

    /** Called when a chord checkbox is toggled. `chordCheckboxStates` is the shared model (mutate pairs, then call updateChordToggleBoxes(chordPanel, chordCheckboxStates)). */
    static void chordToggleChanged(
        int changedIndex,
        std::shared_ptr<std::vector<std::pair<std::string, bool>>> chordCheckboxStates,
        GUIPanel* chordPanel)
    {
        (void)chordPanel;
        if (chordCheckboxStates == nullptr)
        {
            return;
        }
        const std::vector<std::pair<std::string, bool>>& pairs = *chordCheckboxStates;
        std::cout << "chordToggleChanged index=" << changedIndex << " pairs=[";
        for (size_t i = 0; i < pairs.size(); ++i)
        {
            if (i > 0)
            {
                std::cout << ", ";
            }
            std::cout << pairs[i].first << ":" << (pairs[i].second ? "1" : "0");
        }
        std::cout << "]" << std::endl;

        for (size_t i = 0; i < pairs.size(); ++i)
        {
            if (pairs[i].first == "Root")
            {

            }
        }

    }

    static void updateChordToggleBoxes(
        GUIPanel* chordPanel,
        std::shared_ptr<std::vector<std::pair<std::string, bool>>> chordCheckboxStates,
        float startHeightPct = 0.05f)
    {
        if (chordPanel == nullptr || chordCheckboxStates == nullptr)
        {
            return;
        }

        std::vector<std::pair<std::string, bool>>& checkboxStates = *chordCheckboxStates;

        chordPanel->clearChildren();

        const float rowHeightPct = 0.16f;
        const float boxSizePct = 0.10f;
        for (size_t i = 0; i < checkboxStates.size(); ++i)
        {
            const float rowY = startHeightPct + rowHeightPct * static_cast<float>(i);
            if (rowY > 0.90f)
            {
                break;
            }

            // Copy index per row: lambda captures rowIndex by value, so each toggle keeps its own index (not the loop var after the loop).
            const int rowIndex = static_cast<int>(i);
            auto* toggle = new ToggleBox(
                RectSpec{ Pct(0), Pct(rowY), Pct(boxSizePct), Pct(boxSizePct) },
                MarginSpec{ Pct(0), Pct(0), Pct(0), Pct(0) },
                Theme::Control,
                Theme::Accent,
                Theme::AccentBright,
                Theme::ControlHover,
                Theme::AccentBright,
                [chordPanel, rowIndex, chordCheckboxStates](bool)
                {
                    const std::vector<bool> states = collectChordToggleStates(chordPanel);
                    for (size_t j = 0; j < states.size() && j < chordCheckboxStates->size(); ++j)
                    {
                        (*chordCheckboxStates)[j].second = states[j];
                    }
                    chordToggleChanged(rowIndex, chordCheckboxStates, chordPanel);
                });

            auto* label = new PrimitiveText(
                RectSpec{ Pct(0.15f), Pct(rowY), Pct(0.75f), Pct(boxSizePct) },
                MarginSpec{ Pct(0), Pct(0), Pct(0), Pct(0) },
                Pct(0.018f),
                checkboxStates[i].first,
                "C:/Windows/Fonts/segoeui.ttf",
                ALIGN_LEFT,
                VERTICAL_ALIGN_MIDDLE
            );
            label->setFontSizeClamp(12.0f, 18.0f);
            label->setColor(Theme::TextPrimary);

            chordPanel->addChild(toggle);
            toggle->setChecked(checkboxStates[i].second);
            chordPanel->addChild(label);
        }
    }

    static void chordSelected(
        DropdownMenu<std::string>* chordDropdown,
        GUIPanel* chordPanel,
        std::shared_ptr<std::vector<std::pair<std::string, bool>>> chordCheckboxStates)
    {
        if (chordDropdown == nullptr)
        {
            return;
        }

        const std::string* selectedChord = chordDropdown->getSelectedOptionPtr();
        if (selectedChord == nullptr)
        {
            return;
        }

        const std::string& chordType = *selectedChord;
        const int index = chordDropdown->getSelectedIndex();

        static std::string selectedChordType;
        if (selectedChordType == chordType)
        {
            return;
        }
        selectedChordType = chordType;
        updateChordToggleBoxes(chordPanel, chordCheckboxStates, 0.05f);
        std::cout << "Chord selected [" << index << "]: " << chordType << std::endl;
    }

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
                unsigned w = std::max(e.size.width, kGuiWindowMinWidth);
                unsigned h = std::max(e.size.height, kGuiWindowMinHeight);
                if (w != e.size.width || h != e.size.height)
                {
                    guiWindow->setSize(sf::Vector2u(w, h));
                }

                sf::View resizedView(sf::FloatRect(
                    0.0f,
                    0.0f,
                    static_cast<float>(w),
                    static_cast<float>(h)
                ));
                guiWindow->setView(resizedView);

                if (layoutDirty != nullptr)
                {
                    *layoutDirty = true;
                }
            }
            if (e.type == sf::Event::MouseButtonPressed) {
                if (e.mouseButton.button == sf::Mouse::Left) {
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
                else if (e.mouseButton.button == sf::Mouse::Right)
                {
                    if (hoverObject != nullptr)
                    {
                        if (activeObject != nullptr && hoverObject != activeObject)
                        {
                            activeObject->setInactive();
                            activeObject = nullptr;
                        }
                        activeObject = hoverObject;
                        activeObject->setActive();
                        hoverObject->onRightClick();
                    }
                }
            }

            if (e.type == sf::Event::MouseButtonReleased)
            {
                if ((e.mouseButton.button == sf::Mouse::Left || e.mouseButton.button == sf::Mouse::Right) && activeObject != nullptr)
                {
                    activeObject->onMouseButtonReleased(e.mouseButton.button);
                    if (!activeObject->getActive())
                    {
                        activeObject = nullptr;
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
                case sf::Keyboard::Return:
                case sf::Keyboard::Escape:
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

    static void setRadioSelection(std::vector<GUIButton*>& buttons, GUIButton* active)
    {
        for (auto* button : buttons)
        {
            button->setUnselected();
        }
        active->setActive();
    }

    static void initializeRenderObjects(
        std::vector<RenderObject*>& renderObjects, sf::RenderWindow* guiWindow, sf::RenderWindow* gameWindow,
        ToggleBox*& toggle, CircleRingSelect*& circleSelection, FloatSlider*& floatSlider
    )
    {
        (void)gameWindow;


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

        /*auto textBox = new TextBox<std::string>(100, 100, 150, 20, 14, "",
            Theme::Control, Theme::Accent, Theme::Panel, Theme::AccentBright);
        textBox->setWindow(guiWindow);

        auto floatBox = new TextBox<float>(100, 130, 150, 20, 14, "",
            Theme::Control, Theme::Accent, Theme::Panel, Theme::AccentBright);
        floatBox->setWindow(guiWindow);
        floatBox->setValue(420.69f);

        auto intBox = new TextBox<int>(100, 160, 150, 20, 14, "",
            Theme::Control, Theme::Accent, Theme::Panel, Theme::AccentBright);
        intBox->setWindow(guiWindow);
        intBox->setValue(5);

        auto emptyStringBox = new TextBox<std::string>(100, 190, 150, 20, 14, "",
            Theme::Control, Theme::Accent, Theme::Panel, Theme::AccentBright);
        emptyStringBox->setWindow(guiWindow);

        auto textValueBox = new TextBox<std::string>(100, 190, 150, 20, 14, "",
            Theme::Control, Theme::Accent, Theme::Panel, Theme::AccentBright);
        textValueBox->setWindow(guiWindow);
        textValueBox->setValue("This is the string now");

        BoundedFloatSlider* boundedFloatSlider = new BoundedFloatSlider(100, 220, 150, 20,
            Theme::Control, Theme::Accent, Theme::Accent, Theme::TextPrimary, Theme::Panel, Theme::AccentHover, Theme::AccentBright);
        boundedFloatSlider->setWindow(guiWindow);
        boundedFloatSlider->setBounds(0, 360);

        floatSlider = new FloatSlider(100, 250, 150, 20,
            Theme::Control, Theme::Accent, Theme::TextPrimary, Theme::Panel, Theme::AccentBright);
        floatSlider->setWindow(guiWindow);
        floatSlider->setValue(5.0f);
        float slideVal = floatSlider->getValue();
        (void)slideVal;*/
        /*
        textBox->setRectSpec(RectSpec{ Pct(0.14f), Pct(0.16f), Pct(0.18f), Px(24) });
        textBox->setMarginSpec(MarginSpec{ Px(0), Px(0), Px(0), Px(0) });
        // topPanel->addChild(textBox);

        floatBox->setRectSpec(RectSpec{ Pct(0.14f), Pct(0.24f), Pct(0.18f), Px(24) });
        floatBox->setMarginSpec(MarginSpec{ Px(0), Px(0), Px(0), Px(0) });
        // topPanel->addChild(floatBox);

        intBox->setRectSpec(RectSpec{ Pct(0.14f), Pct(0.32f), Pct(0.18f), Px(24) });
        intBox->setMarginSpec(MarginSpec{ Px(0), Px(0), Px(0), Px(0) });
        // topPanel->addChild(intBox);

        emptyStringBox->setRectSpec(RectSpec{ Pct(0.14f), Pct(0.40f), Pct(0.18f), Px(24) });
        emptyStringBox->setMarginSpec(MarginSpec{ Px(0), Px(0), Px(0), Px(0) });
        // topPanel->addChild(emptyStringBox);

        textValueBox->setRectSpec(RectSpec{ Pct(0.14f), Pct(0.48f), Pct(0.22f), Px(24) });
        textValueBox->setMarginSpec(MarginSpec{ Px(0), Px(0), Px(0), Px(0) });
        // topPanel->addChild(textValueBox);

        boundedFloatSlider->setRectSpec(RectSpec{ Pct(0.14f), Pct(0.58f), Pct(0.24f), Px(24) });
        boundedFloatSlider->setMarginSpec(MarginSpec{ Px(0), Px(0), Px(0), Px(0) });
        // topPanel->addChild(boundedFloatSlider);

        floatSlider->setRectSpec(RectSpec{ Pct(0.14f), Pct(0.68f), Pct(0.24f), Px(24) });
        floatSlider->setMarginSpec(MarginSpec{ Px(0), Px(0), Px(0), Px(0) });
        // topPanel->addChild(floatSlider);

        circleSelection = new CircleRingSelect(
            RectSpec{ Pct(0.55f), Pct(0.10f), Pct(0.30f), Pct(0.30f) },
            MarginSpec{ Px(0), Px(0), Px(0), Px(0) },
            Theme::Control, Theme::Control, Theme::Accent, Theme::Accent, Theme::ControlHover, Theme::AccentBright);
        circleSelection->setSlider(boundedFloatSlider);
        // topPanel->addChild(circleSelection);
        */

        auto basePanel = new GUIPanel(
            RectSpec{ Px(0),Px(0),Pct(100),Pct(1.00f) },
            Theme::Panel,
            Theme::BorderPanel,
            0.0f,
            0.0f
        );
        basePanel->setWindow(guiWindow);
        basePanel->setPadding(10.0f);
        
        
        // --- Left panel ---
        {
            auto leftPanel = new GUIPanel(
                RectSpec{ Px(0),Px(0),Pct(25),Pct(100) },
                Theme::PanelAlt,
                Theme::BorderPanel,
                2.0f,
                5.0f
            );
            leftPanel->setPadding(10);
            basePanel->addChild(leftPanel);

            // --- Left panel layout (fractions of the panel content rect) ---

            constexpr float spacing = 0.0125f;
            constexpr float halfSpacing = spacing / 2.0f;
            constexpr float kQuarter = 1.0f / 4.0f;
            constexpr float kSixth = 1.0f / 6.0f;
            constexpr float kRefH = 800.0f;

            // Inset / shared width (inset matches `spacing`)
            constexpr float kLPInset = spacing;
            constexpr float kLPWidth = 0.975f;

            // Duration: title strip, note row, text row (verticals: px / ref height)
            constexpr float kDurTitleH = 28.0f / kRefH;
            constexpr float kDurNoteY = 34.0f / kRefH;
            constexpr float kDurNoteH = 50.0f / kRefH;
            constexpr float kDurTextY = 90.0f / kRefH;
            constexpr float kDurTextH = 24.0f / kRefH;
            constexpr float kDurColW = kQuarter - spacing;

            // Accidentals: title strip, button row (six columns incl. gap)
            constexpr float kAccTitleY = 120.0f / kRefH;
            constexpr float kAccTitleH = 34.0f / kRefH;
            constexpr float kAccRowY = 160.0f / kRefH;
            constexpr float kAccRowH = 35.0f / kRefH;
            constexpr float kAccColW = kSixth - spacing;

            // Chord: title strip, panel
            constexpr float kChordTitleY = 201.0f / kRefH;
            constexpr float kChordTitleH = 32.0f / kRefH;
            constexpr float kChordPanelY = 239.0f / kRefH;
            constexpr float kChordPanelH = 3.0f / 10.0f;

            // Motif: title strip (above panel), panel
            constexpr float kMotifTitleH = 28.0f / kRefH;
            constexpr float kMotifPanelY = 0.65f - halfSpacing;
            constexpr float kMotifPanelH = 1 - kMotifPanelY - spacing/2;
            constexpr float kMotifTitleY = kMotifPanelY - kMotifTitleH - (6.0f / kRefH);

            auto leftPanelTitle = new PrimitiveText(
                RectSpec{ Pct(kLPInset), Pct(0.0f), Pct(kLPWidth), Pct(kDurTitleH) },
                MarginSpec{ Pct(0), Pct(0), Pct(0), Pct(0) },
                Pct(0.02f),
                "Duration:",
                "C:/Windows/Fonts/segoeui.ttf",
                ALIGN_LEFT,
                VERTICAL_ALIGN_BOTTOM
            );
            leftPanelTitle->setFontSizeClamp(16.0f, 24.0f);
            leftPanelTitle->setColor(Theme::TextPrimary);
            leftPanel->addChild(leftPanelTitle);

            auto radioButtons = std::make_shared<std::vector<GUIButton*>>();

            const ImageButtonColors durationNoteColors{
                Theme::Control,
                Theme::ControlHover,
                Theme::ControlPressed,
                Theme::BorderPanel,
                Theme::AccentBright,
                Theme::TextCool,
                Theme::TextCool,
                Theme::TextCool,
            };

            static const char* const kDurationNoteImagePaths[] = {
                "resources/images/sheet_music/notes/Whole.png",
                "resources/images/sheet_music/notes/HalfLow.png",
                "resources/images/sheet_music/notes/QuarterLow.png",
                "resources/images/sheet_music/notes/EighthSingleLow.png",
            };

            static const char* const kDurationNames[] = {
                "16th",
                "32nd",
                "64th",
                "128th",
            };

            for (size_t i = 0; i < 4; ++i)
            {
                auto* noteButton = new ImageButton(
                    RectSpec{ Pct(static_cast<float>(i) * kQuarter + halfSpacing), Pct(kDurNoteY), Pct(kDurColW), Pct(kDurNoteH) },
                    MarginSpec{ Pct(0), Pct(0), Pct(0), Pct(0) },
                    kDurationNoteImagePaths[i],
                    durationNoteColors,
                    nullptr
                );
                if (kDurationNoteImagePaths[i] == "resources/images/sheet_music/notes/Whole.png")
                {
                    noteButton->setImageScaleFactor(0.5f);
                }

                radioButtons->push_back(noteButton);
                noteButton->setOnClick([radioButtons, noteButton]() {
                    setRadioSelection(*radioButtons, noteButton);
                });
                noteButton->setToggleMode(true);
                leftPanel->addChild(noteButton);
            }

            const RectangleButtonColors durationTextColors{
                Theme::Control,
                Theme::ControlHover,
                Theme::ControlPressed,
                Theme::BorderPanel,
                Theme::AccentBright,
                Theme::TextCool,
                Theme::TextCool,
            };

            for (size_t i = 0; i < 4; ++i)
            {
                auto* textButton = new RectangleButton(
                    RectSpec{
                        Pct(static_cast<float>(i) * kQuarter + halfSpacing),
                        Pct(kDurTextY),
                        Pct(kDurColW),
                        Pct(kDurTextH)
                    },
                    MarginSpec{ Pct(0), Pct(0), Pct(0), Pct(0) },
                    durationTextColors,
                    nullptr
                );
                textButton->setFontPath("C:/Windows/Fonts/segoeui.ttf");
                textButton->setText(kDurationNames[i],24);
                radioButtons->push_back(textButton);
                textButton->setOnClick([radioButtons, textButton]() {
                    setRadioSelection(*radioButtons, textButton);
                });
                textButton->setToggleMode(true);
                leftPanel->addChild(textButton);
            }

            setRadioSelection(*radioButtons, (*radioButtons)[0]);

            auto accidentalTitle = new PrimitiveText(
                RectSpec{ Pct(kLPInset), Pct(kAccTitleY), Pct(kLPWidth), Pct(kAccTitleH) },
                MarginSpec{ Pct(0), Pct(0), Pct(0), Pct(0) },
                Pct(0.02f),
                "Accidental:",
                "C:/Windows/Fonts/segoeui.ttf",
                ALIGN_LEFT,
                VERTICAL_ALIGN_BOTTOM
            );
            accidentalTitle->setFontSizeClamp(16.0f, 24.0f);
            accidentalTitle->setColor(Theme::TextPrimary);
            leftPanel->addChild(accidentalTitle);

            auto accidentalButtons = std::make_shared<std::vector<GUIButton*>>();
            static const char* const kAccidentalImagePaths[] = {
                "resources/images/sheet_music/accidentals/double_flat.png",
                "resources/images/sheet_music/accidentals/Flat.png",
                "resources/images/sheet_music/accidentals/Natural.png",
                "resources/images/sheet_music/accidentals/Sharp.png",
                "resources/images/sheet_music/accidentals/Double_Sharp.png",
            };

            const RectangleButtonColors accidentalNoneColors{
                Theme::Control,
                Theme::ControlHover,
                Theme::ControlPressed,
                Theme::BorderPanel,
                Theme::AccentBright,
                Theme::TextCool,
                Theme::TextCool,
            };

            auto* accidentalNoneButton = new RectangleButton(
                RectSpec{ Pct(halfSpacing), Pct(kAccRowY), Pct(kAccColW), Pct(kAccRowH) },
                MarginSpec{ Pct(0), Pct(0), Pct(0), Pct(0) },
                accidentalNoneColors,
                nullptr
            );
            accidentalNoneButton->setFontPath("C:/Windows/Fonts/segoeui.ttf");
            accidentalNoneButton->setText("None", 24);
            accidentalButtons->push_back(accidentalNoneButton);
            accidentalNoneButton->setOnClick([accidentalButtons, accidentalNoneButton]() {
                setRadioSelection(*accidentalButtons, accidentalNoneButton);
            });
            accidentalNoneButton->setToggleMode(true);
            leftPanel->addChild(accidentalNoneButton);

            for (size_t i = 0; i < 5; ++i)
            {
                auto* accidentalButton = new ImageButton(
                    RectSpec{ Pct(static_cast<float>(i + 1) * kSixth + halfSpacing), Pct(kAccRowY), Pct(kAccColW), Pct(kAccRowH) },
                    MarginSpec{ Pct(0), Pct(0), Pct(0), Pct(0) },
                    kAccidentalImagePaths[i],
                    durationNoteColors,
                    nullptr
                );

                if (kAccidentalImagePaths[i] == "resources/images/sheet_music/accidentals/Double_Sharp.png")
                {
                    accidentalButton->setImageScaleFactor(0.6f);
                }

                accidentalButtons->push_back(accidentalButton);
                accidentalButton->setOnClick([accidentalButtons, accidentalButton]() {
                    setRadioSelection(*accidentalButtons, accidentalButton);
                });
                accidentalButton->setToggleMode(true);
                leftPanel->addChild(accidentalButton);
            }

            setRadioSelection(*accidentalButtons, accidentalNoneButton);

            auto chordTitle = new PrimitiveText(
                RectSpec{ Pct(kLPInset), Pct(kChordTitleY), Pct(kLPWidth), Pct(kChordTitleH) },
                MarginSpec{ Pct(0), Pct(0), Pct(0), Pct(0) },
                Pct(0.02f),
                "Chord:",
                "C:/Windows/Fonts/segoeui.ttf",
                ALIGN_LEFT,
                VERTICAL_ALIGN_BOTTOM
            );
            chordTitle->setFontSizeClamp(16.0f, 24.0f);
            chordTitle->setColor(Theme::TextPrimary);
            leftPanel->addChild(chordTitle);

            auto chordPanel = new GUIPanel(
                RectSpec{ Pct(kLPInset), Pct(kChordPanelY), Pct(kLPWidth), Pct(kChordPanelH) },
                Theme::ElevatedPanel,
                Theme::BorderSubtle,
                2.0f,
                5.0f
            );

            //Chord panel split for left and right
            auto chordPanelLeft = new GUIPanel(
                RectSpec{ Pct(0), Pct(0), Pct(50), Pct(100) },
                sf::Color(0, 0, 0, 0),
                sf::Color(0, 0, 0, 0),
                0.0f,
                0.0f
            );

            auto chordPanelRight = new GUIPanel(
                RectSpec{ Pct(55), Pct(0), Pct(45), Pct(100) },
                sf::Color(0, 0, 0, 0),
                sf::Color(0, 0, 0, 0),
                0.0f,
                0.0f
            );

            // CHORD CONTROLS AND TITLES
            {
                auto chordPanelRightTop = new GUIPanel(
                    RectSpec{ Pct(0), Pct(0), Pct(100), Pct(20) },
                    sf::Color(0, 0, 0, 0),
                    sf::Color(0, 0, 0, 0),
                    0.0f,
                    0.0f
                );

                chordPanelRight->addChild(chordPanelRightTop);

                auto chordPanelRightTopTitles = new GUIPanel(
                    RectSpec{ Pct(0), Pct(0), Pct(30), Pct(100) },
                    sf::Color(0, 0, 0, 0),
                    sf::Color(0, 0, 0, 0),
                    0.0f,
                    0.0f
                );
                chordPanelRightTop->addChild(chordPanelRightTopTitles);

                auto chordPanelRightTopControls = new GUIPanel(
                    RectSpec{ Pct(40), Pct(0), Pct(60), Pct(100) },
                    sf::Color(0, 0, 0, 0),
                    sf::Color(0, 0, 0, 0),
                    0.0f,
                    0.0f
                );
                chordPanelRightTop->addChild(chordPanelRightTopControls);

                auto chordRootTitle = new PrimitiveText(
                    RectSpec{ Pct(0), Pct(0), Pct(100), Pct(50) },
                    MarginSpec{ Pct(0), Pct(0), Pct(0), Pct(0) },
                    12,
                    "Root:",
                    "C:/Windows/Fonts/segoeui.ttf",
                    ALIGN_RIGHT,
                    VERTICAL_ALIGN_MIDDLE
                );
                chordRootTitle->setFontSizeClamp(10, 14);

                auto chordOctaveTitle = new PrimitiveText(
                    RectSpec{ Pct(0), Pct(50), Pct(100), Pct(50) },
                    MarginSpec{ Pct(0), Pct(0), Pct(0), Pct(0) },
                    12,
                    "Octave:",
                    "C:/Windows/Fonts/segoeui.ttf",
                    ALIGN_RIGHT,
                    VERTICAL_ALIGN_MIDDLE
                );
                chordOctaveTitle->setFontSizeClamp(10, 14);

                chordPanelRightTopTitles->addChild(chordOctaveTitle);
                chordPanelRightTopTitles->addChild(chordRootTitle);

                const RectangleButtonColors chordStepperColors{
                    Theme::Control,
                    Theme::ControlHover,
                    Theme::ControlPressed,
                    Theme::Accent,
                    Theme::AccentBright,
                    Theme::TextCool,
                    Theme::TextCool,
                };

                // Root row: [button] [textbox] [button]
                auto* rootDecButton = new RectangleButton(
                    RectSpec{ Pct(0.00f), Pct(0.00f), PctH(45), PctH(45) },
                    MarginSpec{ Pct(0), Pct(0), Pct(0), Pct(0) },
                    chordStepperColors,
                    nullptr
                );
                rootDecButton->setFontPath("C:/Windows/Fonts/segoeui.ttf");
                rootDecButton->setText("-", 18);

                auto* rootTextBox = new TextBox<std::string>(
                    0.0f, 0.0f, 0.0f, 0.0f, 16, "C",
                    Theme::Control, Theme::Accent, Theme::ControlHover, Theme::AccentBright,
                    2.0f,
                    [](const std::string& input) -> bool
                    {
                        if (input.empty() || input.size() > 3)
                        {
                            return false;
                        }

                        const char first = static_cast<char>(std::toupper(static_cast<unsigned char>(input[0])));
                        if (first < 'A' || first > 'G')
                        {
                            return false;
                        }

                        if (input.size() == 1)
                        {
                            return true;
                        }

                        const char second = input[1];
                        if (second != '#' && second != 'b')
                        {
                            return false;
                        }

                        if (input.size() == 2)
                        {
                            return true;
                        }

                        const char third = input[2];
                        return (third == '#' || third == 'b') && third == second;
                    }
                    );
                    rootTextBox->setRectSpec(RectSpec{ PctH(0.55f), Pct(0.00f), PctHInv(1.1f), PctH(0.45f) });
                    rootTextBox->setMarginSpec(MarginSpec{ Pct(0), Pct(0), Pct(0), Pct(0) });

                    auto* rootIncButton = new RectangleButton(
                        RectSpec{ PctHInv(0.45f), Pct(0.00f), PctH(45), PctH(45) },
                        MarginSpec{ Pct(0), Pct(0), Pct(0), Pct(0) },
                        chordStepperColors,
                        nullptr
                    );
                    rootIncButton->setFontPath("C:/Windows/Fonts/segoeui.ttf");
                    rootIncButton->setText("+", 18);

                    rootDecButton->setOnClick([rootTextBox]()
                        {
                            RenderUtilities::adjustRootLetter(rootTextBox, false);
                        });
                    rootIncButton->setOnClick([rootTextBox]()
                        {
                            RenderUtilities::adjustRootLetter(rootTextBox, true);
                        });

                    rootDecButton->setOnRightClick([rootTextBox]()
                        {
                            RenderUtilities::adjustRootAccidental(rootTextBox, false);
                        });
                    rootIncButton->setOnRightClick([rootTextBox]()
                        {
                            RenderUtilities::adjustRootAccidental(rootTextBox, true);
                        });

                    // Octave row: [button] [textbox] [button]
                    auto* octaveDecButton = new RectangleButton(
                        RectSpec{ Pct(0.00f), Pct(0.55f), PctH(45), PctH(45) },
                        MarginSpec{ Pct(0), Pct(0), Pct(0), Pct(0) },
                        chordStepperColors,
                        nullptr
                    );
                    octaveDecButton->setFontPath("C:/Windows/Fonts/segoeui.ttf");
                    octaveDecButton->setText("-", 18);

                    auto* octaveTextBox = new TextBox<int>(
                        0.0f, 0.0f, 0.0f, 0.0f, 16, "4",
                        Theme::Control, Theme::Accent, Theme::ControlHover, Theme::AccentBright,
                        2.0f,
                        [](const std::string& input) -> bool
                        {
                            if (input.empty())
                            {
                                return false;
                            }

                            if (!std::all_of(input.begin(), input.end(), [](char c)
                                {
                                    return std::isdigit(static_cast<unsigned char>(c)) != 0;
                                }))
                            {
                                return false;
                            }

                            int octave = 0;
                            try
                            {
                                octave = std::stoi(input);
                            }
                            catch (...)
                            {
                                return false;
                            }

                            return octave >= 0 && octave <= 8;
                        }
                    );
                    octaveTextBox->setRectSpec(RectSpec{ PctH(0.55f), Pct(0.55f), PctHInv(1.1f), PctH(0.45f) });
                    octaveTextBox->setMarginSpec(MarginSpec{ Pct(0), Pct(0), Pct(0), Pct(0) });

                    auto* octaveIncButton = new RectangleButton(
                        RectSpec{ PctHInv(0.45f), Pct(0.55f), PctH(45), PctH(45) },
                        MarginSpec{ Pct(0), Pct(0), Pct(0), Pct(0) },
                        chordStepperColors,
                        nullptr
                    );
                    octaveIncButton->setFontPath("C:/Windows/Fonts/segoeui.ttf");
                    octaveIncButton->setText("+", 18);

                    octaveDecButton->setOnClick([octaveTextBox]()
                        {
                            RenderUtilities::adjustOctave(octaveTextBox, false);
                        });
                    octaveIncButton->setOnClick([octaveTextBox]()
                        {
                            RenderUtilities::adjustOctave(octaveTextBox, true);
                        });

                    chordPanelRightTopControls->addChild(rootDecButton);
                    chordPanelRightTopControls->addChild(rootTextBox);
                    chordPanelRightTopControls->addChild(rootIncButton);
                    chordPanelRightTopControls->addChild(octaveDecButton);
                    chordPanelRightTopControls->addChild(octaveTextBox);
                    chordPanelRightTopControls->addChild(octaveIncButton);
            }

            // CHORD STAFF
            {
                auto chordStaffPanel = new GUIPanel(
                    RectSpec{ Pct(0), Pct(30), Pct(100), Pct(70) },
                    Theme::PanelAlt,
                    Theme::BorderSubtle,
                    2.0f,
                    0.0f
                );

                chordPanelRight->addChild(chordStaffPanel);
            }



            auto chordPanelLeftExtensions = new GUIPanel(
                RectSpec{ Pct(0), Pct(20), Pct(100), Pct(80) },
                sf::Color(0, 0, 0, 0),
                sf::Color(0, 0, 0, 0),
                0.0f,
                0.0f
            );

            chordPanelLeft->addChild(chordPanelLeftExtensions);

            chordPanel->setPadding(15.0f);
            chordPanel->addChild(chordPanelLeft);
            chordPanel->addChild(chordPanelRight);

            leftPanel->addChild(chordPanel);

            const std::vector<std::string> chordTypeOptions{
                "Major",
                "Minor",
                "Diminished",
                "Augmented",
                "Sus2",
                "Sus4",
            };
            auto chordCheckboxStates = std::make_shared<std::vector<std::pair<std::string, bool>>>(
                std::initializer_list<std::pair<std::string, bool>>{
                    { "Root", true },
                    { "Third", true },
                    { "Fifth", true },
                    { "Seventh", false },
                    { "Ninth", false },
                });
            auto* chordTypeDropdown = new DropdownMenu<std::string>(
                RectSpec{ Pct(0.0f), Pct(0.0f), Pct(100), Pct(0.10f) },
                MarginSpec{ Pct(0), Pct(0), Pct(0), Pct(0) },
                12,
                static_cast<int>(chordTypeOptions.size()),
                chordTypeOptions,
                Theme::Control,
                Theme::Accent,
                Theme::ControlHover,
                Theme::AccentBright,
                "Select Chord",
                [chordPanelLeftExtensions, chordCheckboxStates](DropdownMenu<std::string>* dropdown, const std::string&, int)
                {
                    RenderUtilities::chordSelected(dropdown, chordPanelLeftExtensions, chordCheckboxStates);
                });
            chordPanelLeft->addChild(chordTypeDropdown);

            auto motifTitle = new PrimitiveText(
                RectSpec{ Pct(kLPInset), Pct(kMotifTitleY), Pct(kLPWidth), Pct(kMotifTitleH) },
                MarginSpec{ Pct(0), Pct(0), Pct(0), Pct(0) },
                Pct(0.02f),
                "Motif:",
                "C:/Windows/Fonts/segoeui.ttf",
                ALIGN_LEFT,
                VERTICAL_ALIGN_BOTTOM
            );
            motifTitle->setFontSizeClamp(16.0f, 24.0f);
            motifTitle->setColor(Theme::TextPrimary);
            leftPanel->addChild(motifTitle);

            auto motifPanel = new GUIPanel(
                RectSpec{ Pct(kLPInset), Pct(kMotifPanelY), Pct(kLPWidth), Pct(kMotifPanelH) },
                Theme::ElevatedPanel,
                Theme::BorderSubtle,
                2.0f,
                5.0f
            );
            leftPanel->addChild(motifPanel);

        }

        auto rightPanel = new GUIPanel(
            RectSpec{ Pct(80),Pct(0),Pct(20),Pct(100) },
            Theme::ElevatedPanel,
            Theme::BorderSubtle,
            2.0f,
            5.0f
        );
        basePanel->addChild(rightPanel);


        toggle = new ToggleBox(
            RectSpec{ Pct(0.05f), Pct(0.05f), Px(20), Px(20) },
            MarginSpec{ Px(0), Px(0), Px(0), Px(0) },
            Theme::Control, Theme::Accent, Theme::Accent, Theme::ControlHover, Theme::AccentBright);
        toggle->setWindow(guiWindow);
        //leftPanel->addChild(toggle);

        // topPanel->addChild(button);

        /*
        textBox->setRectSpec(RectSpec{ Pct(0.14f), Pct(0.16f), Pct(0.18f), Px(24) });
        textBox->setMarginSpec(MarginSpec{ Px(0), Px(0), Px(0), Px(0) });
        // topPanel->addChild(textBox);

        floatBox->setRectSpec(RectSpec{ Pct(0.14f), Pct(0.24f), Pct(0.18f), Px(24) });
        floatBox->setMarginSpec(MarginSpec{ Px(0), Px(0), Px(0), Px(0) });
        // topPanel->addChild(floatBox);

        intBox->setRectSpec(RectSpec{ Pct(0.14f), Pct(0.32f), Pct(0.18f), Px(24) });
        intBox->setMarginSpec(MarginSpec{ Px(0), Px(0), Px(0), Px(0) });
        // topPanel->addChild(intBox);

        emptyStringBox->setRectSpec(RectSpec{ Pct(0.14f), Pct(0.40f), Pct(0.18f), Px(24) });
        emptyStringBox->setMarginSpec(MarginSpec{ Px(0), Px(0), Px(0), Px(0) });
        // topPanel->addChild(emptyStringBox);

        textValueBox->setRectSpec(RectSpec{ Pct(0.14f), Pct(0.48f), Pct(0.22f), Px(24) });
        textValueBox->setMarginSpec(MarginSpec{ Px(0), Px(0), Px(0), Px(0) });
        // topPanel->addChild(textValueBox);

        boundedFloatSlider->setRectSpec(RectSpec{ Pct(0.14f), Pct(0.58f), Pct(0.24f), Px(24) });
        boundedFloatSlider->setMarginSpec(MarginSpec{ Px(0), Px(0), Px(0), Px(0) });
        // topPanel->addChild(boundedFloatSlider);

        floatSlider->setRectSpec(RectSpec{ Pct(0.14f), Pct(0.68f), Pct(0.24f), Px(24) });
        floatSlider->setMarginSpec(MarginSpec{ Px(0), Px(0), Px(0), Px(0) });
        // topPanel->addChild(floatSlider);
        */


        auto bottomPanel = new GUIPanel(
            RectSpec{ Px(0),Pct(50),Pct(100),Pct(50) },
            Theme::PanelAlt,
            Theme::BorderSubtle,
            2.0f,
            0.0f
        );
        // basePanel->addChild(bottomPanel);

        

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
        // bottomPanel->addChild(staff);



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
        // topPanel->addChild(dropdown);
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

