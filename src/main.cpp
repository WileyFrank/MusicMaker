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
#include "Graphics/GUI/DropdownMenu.h"
#include "Graphics/GUI/Primitives/Rectangle.h"
#include "Graphics/GUI/Primitives/PrimitiveText.h"

#include "Graphics/GameObjects/PlayerObject.h"

#include "Graphics/SheetMusic/SheetMusicElement.h"
#include "Graphics/SheetMusic/SheetMusicClef.h"
#include "Graphics/SheetMusic/SheetMusicMeasure.h"
#include "Helper/RenderUtilities.h"

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

    SoundMixer& mixer = SoundMixer::getInstance();

    mixer.start();
    mixer.setSystem(system);

    FMOD_VECTOR vec = { 0.0f,1.0f,0.0f };
    FMOD_VECTOR vec2 = { 0.0f,1.0f,0.0f };

    sf::RenderWindow gameWindow(sf::VideoMode(1200, 800), "Game Window");
    sf::RenderWindow guiWindow(sf::VideoMode(1200, 800), "Sheet Music Generator");

    //adding objects
    std::vector<RenderObject*> renderObjects;

    //GUI Window Objects referenced in loop
    ToggleBox* toggle;
    CircleRingSelect* circleSelection;
    FloatSlider* floatSlider;

    RenderUtilities::initializeRenderObjects(renderObjects, &guiWindow, &gameWindow, toggle, circleSelection, floatSlider);

    PlayerObject* player = new PlayerObject(500, 100, 256, 256, "resources/game_files/spritesheetAlpha.png", "resources/game_files/spritesheetGlare.png");
    player->setWindow(&gameWindow);
    renderObjects.push_back(player);

    player->setFrame(10);

    PrimitiveText fpsText((float)guiWindow.getSize().x - 20, (float)guiWindow.getSize().y - 30, 24, "FPS: 0", "resources/fonts/Century 751 Bold.otf", ALIGN_RIGHT);
    fpsText.setColor(sf::Color(60, 60, 180));
    fpsText.setWindow(&gameWindow);

    auto currentFrame = std::chrono::high_resolution_clock::now();
    auto previousFrame = std::chrono::high_resolution_clock::now();
    int temp_i = 0;
    auto microsecondCount = 0.0f;

    RenderObject* hoverObject = nullptr;
    RenderObject* previousHoverObject = nullptr;
    RenderObject* activeObject = nullptr;

    std::string inputString;

    auto startTime = std::chrono::high_resolution_clock::now();

    float positionAngle = 0.0f;
    float positionDistance = 0.0f;
    int lastNote = 0;

    std::map<int, FMOD::Sound*> soundMap;
    int FPS60Frame = 0;
    int frameDuration = 0;

    soundMap[0] = MusicUtilities::getSound("Sounds/Piano/c3.wav", system);
    soundMap[1] = MusicUtilities::getSound("Sounds/Piano/d3.wav", system);
    soundMap[2] = MusicUtilities::getSound("Sounds/Piano/e3.wav", system);
    soundMap[3] = MusicUtilities::getSound("Sounds/Piano/f3.wav", system);
    soundMap[4] = MusicUtilities::getSound("Sounds/Piano/g3.wav", system);
    soundMap[5] = MusicUtilities::getSound("Sounds/Piano/a3.wav", system);
    soundMap[6] = MusicUtilities::getSound("Sounds/Piano/b3.wav", system);
    soundMap[7] = MusicUtilities::getSound("Sounds/Piano/c4.wav", system);

    gameWindow.close();
    while (guiWindow.isOpen() || gameWindow.isOpen())
    {
        //Frame data
        temp_i++;
        currentFrame = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(currentFrame - previousFrame);
        previousFrame = std::chrono::high_resolution_clock::now();
        frameDuration += (int)duration.count();

        //player->update((float)duration.count() / 1000000.0f);

        if (frameDuration > 1000000 / 60)
        {
            //called at 60fps
            FPS60Frame += (int) frameDuration / (1000000 / 60);

            //Footsteps TODO: Separate into beat controlled sound player
            /*if (FPS60Frame % 30 == 0)
            {
                auto& sound = mixer.addSound("resources/game_files/Sounds/footstep.wav");
                sound.setVolume(0.10);
            }*/

            //FPS60Frame ++ ;
            frameDuration -= 1000000 / 60;
            player->setFrame(FPS60Frame);
        }

        microsecondCount += duration.count();

        //FPS Calculations
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
        RenderUtilities::setActiveHover(renderObjects, hoverObject, previousHoverObject, activeObject);
        //Event Polling for both windows
        RenderUtilities::pollEvents(&guiWindow, &gameWindow, player, activeObject, hoverObject);
      
        // Clear screen
        guiWindow.clear(sf::Color(0,3,25));
        gameWindow.clear(sf::Color(0, 3, 25));

        fpsText.draw();
        RenderUtilities::drawRenderObjects(renderObjects, hoverObject, activeObject);

        if (toggle->getState())
        {
            RenderUtilities::playScale(mixer, soundMap, positionAngle, positionDistance,
                circleSelection, floatSlider, lastNote, startTime);
        }

        // Update the window
        guiWindow.display();
        //gameWindow.display();
    }

    mixer.stop();

    MusicUtilities::releaseAllCachedSounds();

    return 0;
}