#pragma once
#include "../GUI/GUIUtilities.h"
#include "../ResourceManager.h"
#include "../../HeaderFiles/SoundMixer.h"
#include "AnimationUtilities.h"



class PlayerObject :
    public RenderObject
{

private:
    SoundMixer* soundMixer;
    
    sf::Texture* playerTexture;
    sf::Texture* lightTexture;
    sf::Sprite playerSprite;
    sf::Sprite lightSprite;

    SpriteSheet playerSheet, lightSheet;

    sf::RenderStates drawStates;


    //The time that the last frame was updated in the global framerate
    std::chrono::steady_clock::time_point frameTime;


    std::chrono::steady_clock::time_point currentTime;
    std::chrono::steady_clock::time_point previousTime;

    //move speed in in pixels per second
    float deltaTime;

    float runSpeed;
    int frames, currentFrame, previousFrame, fps, fps60, secondCount;

    //sprite sheet variables
    int spriteWidth, spriteHeight;
    int spriteX, spriteY;


    //state variables
    bool facingRight;
    PlayerState playerState;
    

    void initializeSpritesheets();

    void handleInputs();

    //Sprite modification
    void updateSprites();

    void setSpriteFrame();
    void mirrorSpriteHorizontal();
public:
    PlayerObject(float x, float y, float width, float height, std::string playerTexturePath, std::string lightTexturePath);

    void setFrame(int frame) { currentFrame = (int)((float)frame / (60.0 / fps)) % frames; fps60 = frame; }
    void render() override;
    void update() override;

    void draw() override;

    void arrowKeyInput(sf::Keyboard::Key key) override;

    RenderObject& getHoverObject() override { return *GUIUtilities::getEmptyRenderObject(); }
};

