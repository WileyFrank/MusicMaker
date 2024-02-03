#pragma once

enum PlayerState {
    IDLE_RIGHT = -2,
    IDLE_LEFT = -1,
    RUNNING_LEFT = 1,
    RUNNING_RIGHT = 2
};

enum AnimationState
{
    ANIMATION_NORMAL = 0,
    ANIMATION_TRANSITION,
    ANIMATION_OFFSET
};

struct Animation {
    int x, y, frames;
    int width, height;
    int dbi;
};

class OffsetTransitionAnimation
{
private:
    Animation *transition, *offset;
    Animation *previousAnimation, *nextAnimation;

    std::vector<int> offsetFrames = {};
    int startFrame = -1;
    int currentFrame = 0;
    int previousFrame = -1;
    int framesRemaining = -1;
    int frameWidth;

    int nextAnimationStart;
public:
    OffsetTransitionAnimation(Animation* previousAnimation, Animation* nextAnimation,
        Animation* transition, Animation* offset, int nextAnimationStart)
        :transition(transition), offset(offset), 
        previousAnimation(previousAnimation), nextAnimation(nextAnimation),
        nextAnimationStart(nextAnimationStart)
    {
        for (int i = 0; i < offset->frames; i++)
        {
            offsetFrames.push_back(i);
        }
    } 

    void setOffsetFrames(std::vector<int> frames)
    {
        offsetFrames.clear();

        for (int i : frames)
        {
            offsetFrames.push_back(i);
        }
    }

    void setStartFrame(int startFrame) { this->startFrame = startFrame; }

    void setFrame(int frame) {
        this->currentFrame = frame; 
        if (startFrame == -1 || startFrame > currentFrame) 
        { 
            startFrame = currentFrame; 
        }

        framesRemaining = ((int)offsetFrames.size() + transition->frames) - (currentFrame - startFrame);
    }

    void setFrameCount(int frameCount) { framesRemaining = frameCount; }
    int getFramesRemaining() { return framesRemaining; }

    void setFrameWidth(int frameWidth) { this->frameWidth = frameWidth; }

    int getTransitionFrames() { return transition->frames; }
    int getNextAnimationStart() { return nextAnimationStart; }

    sf::IntRect getSpriteRect() 
    {
        //If in offset before transition
        if (currentFrame - startFrame < offsetFrames.size())
        {
            int frame = currentFrame - startFrame;
            if (currentFrame != previousFrame)
            {
                /*std::cout << currentFrame << ", " << startFrame << ", " << frame << "\t\t";
                std::cout << "returning offset : " << offsetFrames[frame] << std::endl;*/
                previousFrame = currentFrame;
            }

            int spriteX = ((offset->x + offsetFrames[frame]) % frameWidth) * offset->width;
            int spriteY = (offset->y + (int)(offsetFrames[frame] / frameWidth)) * offset->height;


            return sf::IntRect(spriteX, spriteY,
                offset->width, offset->height);
        }
        else
        {
            int frame = currentFrame - startFrame - (int)offsetFrames.size();
            if (currentFrame != previousFrame)
            {
                std::cout << currentFrame << ", " << startFrame << ", " << frame << "\t\t";
                std::cout << "returning transition : " << frame << std::endl;
                previousFrame = currentFrame;
            }
            int spriteX = (transition->x + (frame % frameWidth)) * transition->width;
            int spriteY = (transition->y + (int)(frame / frameWidth)) * transition->height;


            return sf::IntRect(spriteX, spriteY,
                transition->width, transition->height);
        }
    }

    AnimationState getAnimationState()
    {
        if (startFrame == -1 || framesRemaining <= 0)
        {
            return ANIMATION_NORMAL;
        }
        
        if (currentFrame - startFrame < offsetFrames.size())
        {
            return ANIMATION_OFFSET;
        }

        return ANIMATION_TRANSITION;
    }

};

class SpriteSheet
{
public:
    int frameWidth, frameHeight;
    int currentAnimation, currentFrame, previousFrame, fps;
    int frameOffset;


    int previousAnimation, nextAnimation;

    std::string path;

    // tracks the time since the first frame
    float time;

    std::map<int, Animation> animations;
    std::map<int, Animation> transitionAnimations;
    std::map<int, Animation> offsetAnimations;
    std::map<int, std::map<int, OffsetTransitionAnimation*>> offsetTransitionAnimations;

    bool inTransition;

    sf::Sprite sprite;
    sf::Texture* texture;
    sf::RenderStates drawStates;

    SpriteSheet(int width, int height, int fps, std::string path)
        :frameWidth(width), frameHeight(height), fps(fps), path(path), inTransition(false)
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
    void addTransitionAnimation(int start, int end, Animation transition, Animation offset, int frameStart)
    {
        int mapIndex = start * 10 + end;

        transitionAnimations[mapIndex] = transition;
        offsetAnimations[mapIndex] = offset;

        int currentFrameDBI = currentFrame % animations[end].dbi;
        int toDB = 8 - currentFrameDBI;

        OffsetTransitionAnimation* ota = new OffsetTransitionAnimation(&animations[start], &animations[end],
            &transitionAnimations[mapIndex], &offsetAnimations[mapIndex], frameStart);
        ota->setFrameWidth(frameWidth);

        offsetTransitionAnimations[start][end] = ota;
    }

    void setAnimation(int id) {
        if (animations.find(id) != animations.end()) {
            currentAnimation = id;
            if (previousAnimation != currentAnimation)
            {
                if (!inTransition)
                {
                    std::cout << "set animation to : " << id << std::endl;
                    if (offsetTransitionAnimations.find(previousAnimation) != offsetTransitionAnimations.end() &&
                        offsetTransitionAnimations[previousAnimation].find(currentAnimation) != offsetTransitionAnimations[previousAnimation].end())
                    {

                        setTransitionAnimation(previousAnimation, currentAnimation);
                        currentAnimation = previousAnimation;

                    }
                    else
                    {
                        previousAnimation = currentAnimation;
                    }
                }
            }
        }
        else
        {
            std::cout << "Animation not found. ID: " << id << std::endl;
        }
    }
    void setTransitionAnimation(int start, int end)
    {
        if (offsetTransitionAnimations.find(start) != offsetTransitionAnimations.end())
        {
            if (offsetTransitionAnimations[start].find(end) != offsetTransitionAnimations[start].end())
            {
                inTransition = true;
                previousAnimation = start;
                nextAnimation = end;

                OffsetTransitionAnimation* ota = offsetTransitionAnimations[start][end];

                int currentFrameDBI = (currentFrame + frameOffset) % animations[end].dbi;
                int toDB = 8 - currentFrameDBI;

                std::cout << "Frames to DB: " << toDB << std::endl;
                //the minimum amount of frames before the next downbeat
                int minTransition = ota->getTransitionFrames() + (ota->getNextAnimationStart() % animations[end].dbi);

                int frameOffset = (toDB - minTransition + animations[end].dbi) % animations[end].dbi;

                std::vector<int> offsetFrames;

                for (int i = 0; i < frameOffset; i++)
                {
                    //0 1 2 3 4 5 6
                    //becomes
                    //0 1 2 3 2 1 0
                    if (i <= frameOffset / 2)
                    {
                        offsetFrames.push_back(i);
                    }
                    else
                    {
                        offsetFrames.push_back((frameOffset - i) - frameOffset % 2);
                    }
                }

                ota->setOffsetFrames(offsetFrames);

                std::cout << "setStartSTA\n";
                ota->setStartFrame(currentFrame);

                return;
            }
        }

        std::cout << "Transition animation not found. \tStart : " << start << "\t\tEnd : " << end << std::endl;
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
        currentFrame = ((frame * fps) / basefps);
        if (inTransition)
        {
            offsetTransitionAnimations[previousAnimation][nextAnimation]->setFrame(currentFrame);
            if (offsetTransitionAnimations[previousAnimation][nextAnimation]->getFramesRemaining() <= 0)
            {
                currentAnimation = nextAnimation;

                std::cout << "setStart Frame\n";

                
                int nextAnimationStart = offsetTransitionAnimations[previousAnimation][nextAnimation]->getNextAnimationStart();

                updateOffset(nextAnimationStart, currentAnimation);
                
                previousAnimation = currentAnimation;
                setAnimation(currentAnimation);
                inTransition = false;

            }
        }

        if (currentFrame != previousFrame)
        {
            //std::cout << "Current frame: " << currentFrame << std::endl;
            //std::cout << "Offset frames: " << frameOffset << std::endl;
            previousFrame = currentFrame;
        }


        //If currently in transition animation or frame handled elsewhere
        if (inTransition)
        {
            sprite.setTextureRect(offsetTransitionAnimations[previousAnimation][nextAnimation]->getSpriteRect());
        }
        else
        {
            int spriteFrame = (currentFrame + frameOffset) % animations[currentAnimation].frames;
            int spriteX = (animations[currentAnimation].x + spriteFrame % frameWidth) * animations[currentAnimation].width;
            int spriteY = (animations[currentAnimation].y + (int)(spriteFrame / frameWidth)) * animations[currentAnimation].height;
            sprite.setTextureRect(sf::IntRect(spriteX, spriteY,
                animations[currentAnimation].width, animations[currentAnimation].height));
        }

    }

    void updateOffset(int startFrame, int animation)
    {
        int currentSpriteFrame = (currentFrame + frameOffset) % animations[animation].frames;

        if (currentSpriteFrame != startFrame)
        {
            frameOffset = ((startFrame - currentFrame % animations[animation].frames) + animations[currentAnimation].frames);
            frameOffset %= animations[currentAnimation].frames;
            if (frameOffset % animations[currentAnimation].dbi != 0)
            {
                std::cout << "ERROR IN OFFSET\n";
            }
            std::cout << "Value of offset: " << frameOffset << std::endl;
        }
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

    AnimationState getAnimationState() {
        if (!inTransition)
        {
            return ANIMATION_NORMAL;
        }
        else
        {
            return offsetTransitionAnimations[previousAnimation][nextAnimation]->getAnimationState();
        }
    }

    ~SpriteSheet() {
        for (auto& outerPair : offsetTransitionAnimations) {
            for (auto& innerPair : outerPair.second) {
                delete innerPair.second;
            }
            outerPair.second.clear();
        }
    }
};



class AnimationUtilities
{

};