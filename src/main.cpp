#include "HeaderFiles/MainLoopHelpers.h"

int main() {
    FMOD::System* system = MainLoopHelpers::initializeFmodSystem();
    MainLoopHelpers::initializeListener(system);

    SoundMixer& mixer = SoundMixer::getInstance();
    MainLoopHelpers::initializeSoundMixer(mixer, system);
    auto soundMap = MainLoopHelpers::loadPianoScale(system);

    sf::RenderWindow gameWindow(sf::VideoMode(1200, 800), "Game Window");
    sf::RenderWindow guiWindow(sf::VideoMode(1200, 800), "Sheet Music Generator");

    std::vector<RenderObject*> renderObjects;
    ToggleBox* toggle = nullptr;
    CircleRingSelect* circleSelection = nullptr;
    FloatSlider* floatSlider = nullptr;
    PlayerObject* player = nullptr;

    MainLoopHelpers::initializeScene(
        renderObjects,
        guiWindow,
        gameWindow,
        toggle,
        circleSelection,
        floatSlider,
        player
    );
    PrimitiveText fpsText = MainLoopHelpers::initializeFpsText(guiWindow, gameWindow);

    MainLoopHelpers::FrameState frameState;
    MainLoopHelpers::InteractionState interactionState;

    while (guiWindow.isOpen() || gameWindow.isOpen())
    {
        std::chrono::nanoseconds duration(0);
        MainLoopHelpers::updateFrameState(frameState, duration);
        MainLoopHelpers::runFixedUpdate(frameState, player);
        MainLoopHelpers::updateFpsText(frameState, fpsText);
        MainLoopHelpers::processInput(renderObjects, interactionState, guiWindow, gameWindow, player);
        MainLoopHelpers::updateScalePlayback(toggle, mixer, soundMap, interactionState, circleSelection, floatSlider);
        MainLoopHelpers::renderFrame(guiWindow, gameWindow, fpsText, renderObjects, interactionState);
        MainLoopHelpers::throttleLoopToClock(frameState);
    }

    MainLoopHelpers::shutdownAudio(mixer);

    return 0;
}