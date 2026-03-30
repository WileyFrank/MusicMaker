#include "HeaderFiles/MainLoopHelpers.h"

int main() {
    FMOD::System* system = MainLoopHelpers::initializeFmodSystem();
    MainLoopHelpers::initializeListener(system);

    SoundMixer& mixer = SoundMixer::getInstance();
    MainLoopHelpers::initializeSoundMixer(mixer, system);
    auto soundMap = MainLoopHelpers::loadPianoScale(system);

    sf::RenderWindow gameWindow(sf::VideoMode(1200, 800), "Game Window");
    sf::RenderWindow guiWindow(sf::VideoMode(1200, 800), "Sheet Music Generator");

    // Hide the game window so only the GUI is visible.
    // To bring it back, change `false` to `true` (or remove this line).
    gameWindow.setVisible(false);

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
    // FPS text was originally drawn to `gameWindow`; redirect to the visible GUI window.
    fpsText.setWindow(&guiWindow);

    MainLoopHelpers::FrameState frameState;
    MainLoopHelpers::InteractionState interactionState;
    bool layoutDirty = true;

    while (guiWindow.isOpen() || gameWindow.isOpen())
    {
        if (layoutDirty)
        {
            MainLoopHelpers::updateUiLayout(renderObjects, guiWindow);
            layoutDirty = false;
        }

        std::chrono::nanoseconds duration(0);
        MainLoopHelpers::updateFrameState(frameState, duration);
        MainLoopHelpers::runFixedUpdate(frameState, player);
        MainLoopHelpers::updateFpsText(frameState, fpsText);
        MainLoopHelpers::processInput(renderObjects, interactionState, guiWindow, gameWindow, layoutDirty, player);
        MainLoopHelpers::updateScalePlayback(toggle, mixer, soundMap, interactionState, circleSelection, floatSlider);
        MainLoopHelpers::renderFrame(guiWindow, gameWindow, fpsText, renderObjects, interactionState);
        MainLoopHelpers::throttleLoopToClock(frameState);
    }

    MainLoopHelpers::shutdownAudio(mixer);

    return 0;
}