#include "HeaderFiles/MainLoopHelpers.h"

#include <iostream>
#include <thread>
#include <cmath>
#include "Music/MusicUtilities.h"
#include "Helper/RenderUtilities.h"

namespace {
constexpr std::chrono::milliseconds kSleepSlack(2);

void checkFmodResult(FMOD_RESULT result) {
    if (result != FMOD_OK) {
        std::cerr << "FMOD error: " << result << std::endl;
        exit(-1);
    }
}
} // namespace

namespace MainLoopHelpers {
FMOD::System* initializeFmodSystem() {
    FMOD::System* system = nullptr;
    checkFmodResult(FMOD::System_Create(&system));
    checkFmodResult(system->init(512, FMOD_INIT_NORMAL, nullptr));
    return system;
}

void initializeListener(FMOD::System* system) {
    FMOD_VECTOR listenerPos = { 0.0f, 0.0f, 0.0f };
    FMOD_VECTOR forward = { 0.0f, 1.0f, 0.0f };
    FMOD_VECTOR up = { 0.0f, 0.0f, 1.0f };
    FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };
    checkFmodResult(system->set3DListenerAttributes(0, &listenerPos, &velocity, &forward, &up));
}

void initializeSoundMixer(SoundMixer& mixer, FMOD::System* system) {
    mixer.start();
    mixer.setSystem(system);
}

std::map<int, FMOD::Sound*> loadPianoScale(FMOD::System* system) {
    std::map<int, FMOD::Sound*> soundMap;
    soundMap[0] = MusicUtilities::getSound("Sounds/Piano/c3.wav", system);
    soundMap[1] = MusicUtilities::getSound("Sounds/Piano/d3.wav", system);
    soundMap[2] = MusicUtilities::getSound("Sounds/Piano/e3.wav", system);
    soundMap[3] = MusicUtilities::getSound("Sounds/Piano/f3.wav", system);
    soundMap[4] = MusicUtilities::getSound("Sounds/Piano/g3.wav", system);
    soundMap[5] = MusicUtilities::getSound("Sounds/Piano/a3.wav", system);
    soundMap[6] = MusicUtilities::getSound("Sounds/Piano/b3.wav", system);
    soundMap[7] = MusicUtilities::getSound("Sounds/Piano/c4.wav", system);
    return soundMap;
}

void initializeScene(
    std::vector<RenderObject*>& renderObjects,
    sf::RenderWindow& guiWindow,
    sf::RenderWindow& gameWindow,
    ToggleBox*& toggle,
    CircleRingSelect*& circleSelection,
    FloatSlider*& floatSlider,
    PlayerObject*& player
) {
    RenderUtilities::initializeRenderObjects(renderObjects, &guiWindow, &gameWindow, toggle, circleSelection, floatSlider);
    player = new PlayerObject(500, 100, 256, 256, "resources/game_files/spritesheetAlpha.png", "resources/game_files/spritesheetGlare.png");
    player->setWindow(&gameWindow);
    player->setFrame(10);
    renderObjects.push_back(player);
}

PrimitiveText initializeFpsText(sf::RenderWindow& guiWindow, sf::RenderWindow& gameWindow) {
    PrimitiveText fpsText(
        (float)guiWindow.getSize().x - 20,
        (float)guiWindow.getSize().y - 30,
        24,
        "FPS: 0",
        "resources/fonts/Century 751 Bold.otf",
        ALIGN_RIGHT
    );
    fpsText.setColor(sf::Color(60, 60, 180));
    fpsText.setWindow(&gameWindow);
    return fpsText;
}

void updateFrameState(FrameState& frameState, std::chrono::nanoseconds& duration) {
    frameState.frameCounter++;
    frameState.currentFrame = Clock::now();
    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(frameState.currentFrame - frameState.previousFrame);
    frameState.previousFrame = frameState.currentFrame;
    frameState.fixedStepAccumulator += duration;
    frameState.nanosecondAccumulator += static_cast<double>(duration.count());
}

void runFixedUpdate(FrameState& frameState, PlayerObject* player) {
    while (frameState.fixedStepAccumulator >= kFrameStep)
    {
        frameState.animationFrame++;
        player->setFrame(frameState.animationFrame);
        frameState.fixedStepAccumulator -= kFrameStep;
    }
}

void updateFpsText(FrameState& frameState, PrimitiveText& fpsText) {
    if (frameState.frameCounter % 200 == 0)
    {
        const double averageDurationNs = frameState.nanosecondAccumulator / 200.0;
        frameState.nanosecondAccumulator = 0.0;
        const double fps = 1000000000.0 / averageDurationNs;
        std::string fpsString = "FPS: " + std::to_string((int)std::lround(fps));
        fpsText.setText(fpsString);
    }
}

void throttleLoopToClock(FrameState& frameState) {
    const auto now = Clock::now();
    const auto sleepTarget = frameState.nextFrameDeadline - kSleepSlack;
    if (now < sleepTarget) {
        std::this_thread::sleep_until(sleepTarget);
    }

    while (Clock::now() < frameState.nextFrameDeadline) {
        std::this_thread::yield();
    }

    frameState.nextFrameDeadline += kFrameStep;
    const auto afterWait = Clock::now();
    if (frameState.nextFrameDeadline <= afterWait) {
        frameState.nextFrameDeadline = afterWait + kFrameStep;
    }
}

void processInput(
    std::vector<RenderObject*>& renderObjects,
    InteractionState& interactionState,
    sf::RenderWindow& guiWindow,
    sf::RenderWindow& gameWindow,
    bool& layoutDirty,
    PlayerObject*& player
) {
    RenderUtilities::setActiveHover(
        renderObjects,
        interactionState.hoverObject,
        interactionState.previousHoverObject,
        interactionState.activeObject
    );
    RenderUtilities::pollEvents(&guiWindow, &gameWindow, player, interactionState.activeObject, interactionState.hoverObject, &layoutDirty);
    if (layoutDirty)
    {
        RenderUtilities::updateUiLayout(renderObjects, &guiWindow);
        layoutDirty = false;
    }
}

void updateUiLayout(std::vector<RenderObject*>& renderObjects, sf::RenderWindow& guiWindow) {
    RenderUtilities::updateUiLayout(renderObjects, &guiWindow);
}

void renderFrame(
    sf::RenderWindow& guiWindow,
    sf::RenderWindow& gameWindow,
    PrimitiveText& fpsText,
    std::vector<RenderObject*>& renderObjects,
    InteractionState& interactionState
) {
    guiWindow.clear(sf::Color(0, 3, 25));
    gameWindow.clear(sf::Color(0, 3, 25));
    fpsText.draw();
    RenderUtilities::drawRenderObjects(renderObjects, interactionState.hoverObject, interactionState.activeObject);
    guiWindow.display();
    gameWindow.display();
}

void updateScalePlayback(
    ToggleBox* toggle,
    SoundMixer& mixer,
    std::map<int, FMOD::Sound*>& soundMap,
    InteractionState& interactionState,
    CircleRingSelect*& circleSelection,
    FloatSlider*& floatSlider
) {
    if (toggle == nullptr || circleSelection == nullptr || floatSlider == nullptr)
    {
        return;
    }

    if (toggle->getState())
    {
        RenderUtilities::playScale(
            mixer,
            soundMap,
            interactionState.positionAngle,
            interactionState.positionDistance,
            circleSelection,
            floatSlider,
            interactionState.lastNote,
            interactionState.startTime
        );
    }
}

void shutdownAudio(SoundMixer& mixer) {
    mixer.stop();
    MusicUtilities::releaseAllCachedSounds();
}
} // namespace MainLoopHelpers

