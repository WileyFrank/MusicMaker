#pragma once

enum PlayerState {
    IDLE_RIGHT = -2,
    IDLE_LEFT = -1,
    RUNNING_LEFT = 1,
    RUNNING_RIGHT = 2
};

struct Animation {
    int x, y, frames;
    int width, height;
};


class SpriteSheet
{
public:
    int frameWidth, frameHeight;
    int currentAnimation, currentFrame, previousFrame, fps;

    std::string path;

    // tracks the time since the first frame
    float time;

    std::map<int, Animation> animations;

    sf::Sprite sprite;
    sf::Texture* texture;
    sf::RenderStates drawStates;

    SpriteSheet(int width, int height, int fps, std::string path)
        :frameWidth(width), frameHeight(height), fps(fps), path(path)
    {
        texture = ResourceManager::getTexture(path);
        sprite.setTexture(*texture);
        drawStates.blendMode = sf::BlendNone;

    }

    void setBlendMode(sf::BlendMode mode) {
        drawStates.blendMode = mode;
    }

    void addAnimation(int id, Animation animation) {
        // Store the pointer in the map
        animations[id] = animation;
    }

    void setAnimation(int id) {
        if (animations.find(id) != animations.end()) {
            currentAnimation = id;
            // Reset the animation state if needed
        }
    }

    void setFrame(int frame)
    {
        previousFrame = frame;
        int spriteX = animations[currentAnimation].x + (animations[currentAnimation].width * frame);
        int spriteY = animations[currentAnimation].y + (animations[currentAnimation].height * (int)(frame / frameWidth));

        sprite.setTextureRect(sf::IntRect(spriteX, spriteY,
            animations[currentAnimation].width, animations[currentAnimation].height));
    }

    void setFrame(int frame, int basefps)
    {
        currentFrame = ((frame * fps) / basefps) % animations[currentAnimation].frames;

        previousFrame = currentFrame;
        int spriteX = (animations[currentAnimation].x + currentFrame % frameWidth) * animations[currentAnimation].width;
        int spriteY = (animations[currentAnimation].y + (int)(currentFrame / frameWidth)) * animations[currentAnimation].height;

        sprite.setTextureRect(sf::IntRect(spriteX, spriteY,
            animations[currentAnimation].width, animations[currentAnimation].height));
    }

    void update(float deltaTime) {
        // Update the animation frame based on deltaTime
        // Change the texture rectangle of the sprite

        time += deltaTime;

        currentFrame = (int)(time / (1 / (float)24)) % animations[currentAnimation].frames;

        if (previousFrame != currentFrame)
        {
            setFrame(currentFrame);            
        }
    }

    void draw(sf::RenderWindow& window)
    {
        if (drawStates.blendMode != sf::BlendNone)
        {
            window.draw(sprite, drawStates);
        }
        else
        {
            window.draw(sprite);
        }

    }

    void setPosition(sf::Vector2f position)
    {
        sprite.setPosition(position);
    }

    void setScale(float xScale, float yScale)
    {
        sprite.setScale(sf::Vector2f(xScale, yScale));
    }

    ~SpriteSheet() {
    }
};



class AnimationUtilities
{

};