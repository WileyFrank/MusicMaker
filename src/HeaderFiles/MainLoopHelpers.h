#pragma once

#include <chrono>
#include <map>
#include <vector>
#include <fmod.hpp>
#include "SFML/Graphics.hpp"
#include "../Graphics/GUI/RenderObject.h"
#include "../Graphics/GUI/FloatSlider.h"
#include "../Graphics/GUI/CircleRingSelect.h"
#include "../Graphics/GUI/ToggleBox.h"
#include "../Graphics/GUI/Primitives/PrimitiveText.h"
#include "../Graphics/GameObjects/PlayerObject.h"
#include "SoundMixer.h"

namespace MainLoopHelpers {
using Clock = std::chrono::steady_clock;
constexpr std::chrono::nanoseconds kFrameStep(1000000000LL / 60);

struct FrameState {
    Clock::time_point currentFrame = Clock::now();
    Clock::time_point previousFrame = Clock::now();
    Clock::time_point nextFrameDeadline = Clock::now() + kFrameStep;
    int frameCounter = 0;
    double nanosecondAccumulator = 0.0;
    int animationFrame = 0;
    std::chrono::nanoseconds fixedStepAccumulator = std::chrono::nanoseconds::zero();
};

struct InteractionState {
    RenderObject* hoverObject = nullptr;
    RenderObject* previousHoverObject = nullptr;
    RenderObject* activeObject = nullptr;
    float positionAngle = 0.0f;
    float positionDistance = 0.0f;
    int lastNote = 0;
    Clock::time_point startTime = Clock::now();
};

FMOD::System* initializeFmodSystem();
void initializeListener(FMOD::System* system);
void initializeSoundMixer(SoundMixer& mixer, FMOD::System* system);
std::map<int, FMOD::Sound*> loadPianoScale(FMOD::System* system);

void initializeScene(
    std::vector<RenderObject*>& renderObjects,
    sf::RenderWindow& guiWindow,
    sf::RenderWindow& gameWindow,
    ToggleBox*& toggle,
    CircleRingSelect*& circleSelection,
    FloatSlider*& floatSlider,
    PlayerObject*& player
);

PrimitiveText initializeFpsText(sf::RenderWindow& guiWindow, sf::RenderWindow& gameWindow);
void updateFrameState(FrameState& frameState, std::chrono::nanoseconds& duration);
void runFixedUpdate(FrameState& frameState, PlayerObject* player);
void updateFpsText(FrameState& frameState, PrimitiveText& fpsText);
void throttleLoopToClock(FrameState& frameState);

void processInput(
    std::vector<RenderObject*>& renderObjects,
    InteractionState& interactionState,
    sf::RenderWindow& guiWindow,
    sf::RenderWindow& gameWindow,
    bool& layoutDirty,
    PlayerObject*& player
);

void updateUiLayout(std::vector<RenderObject*>& renderObjects, sf::RenderWindow& guiWindow);

void renderFrame(
    sf::RenderWindow& guiWindow,
    sf::RenderWindow& gameWindow,
    PrimitiveText& fpsText,
    std::vector<RenderObject*>& renderObjects,
    InteractionState& interactionState
);

void updateScalePlayback(
    ToggleBox* toggle,
    SoundMixer& mixer,
    std::map<int, FMOD::Sound*>& soundMap,
    InteractionState& interactionState,
    CircleRingSelect*& circleSelection,
    FloatSlider*& floatSlider
);

void shutdownAudio(SoundMixer& mixer);
} // namespace MainLoopHelpers

