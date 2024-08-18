#include "PlayerObject.h"

void PlayerObject::initializeSpritesheets()
{
	Animation newAnimation = Animation({ 0, 0, 16, 256, 256, 8 });
	playerSheet.addAnimation(RUNNING_RIGHT, newAnimation);
	lightSheet.addAnimation(RUNNING_RIGHT, newAnimation);

	newAnimation = Animation({ 0, 1, 16, 256, 256, 8 });
	playerSheet.addAnimation(RUNNING_LEFT, newAnimation);
	lightSheet.addAnimation(RUNNING_LEFT, newAnimation);

	newAnimation = Animation({ 0, 2, 32, 256, 256, 16 });
	playerSheet.addAnimation(IDLE_RIGHT, newAnimation);
	lightSheet.addAnimation(IDLE_RIGHT, newAnimation);

	newAnimation = Animation({ 0, 4, 32, 256, 256, 16});
	playerSheet.addAnimation(IDLE_LEFT, newAnimation);
	lightSheet.addAnimation(IDLE_LEFT, newAnimation);

	Animation transition = Animation({ 7, 6, 3, 256, 256, 0 });
	Animation offset = Animation({ 10, 6, 4, 256, 256, 0 });
	playerSheet.addTransitionAnimation(IDLE_LEFT, RUNNING_LEFT, transition, offset, 12);
	lightSheet.addTransitionAnimation(IDLE_LEFT, RUNNING_LEFT, transition, offset, 12);

	transition = Animation({ 0, 6, 3, 256, 256, 0 });
	offset = Animation({ 3, 6, 4, 256, 256, 0 });
	playerSheet.addTransitionAnimation(IDLE_RIGHT, RUNNING_RIGHT, transition, offset, 12);
	lightSheet.addTransitionAnimation(IDLE_RIGHT, RUNNING_RIGHT, transition, offset, 12);


	transition = Animation({ 3, 7, 3, 256, 256, 0 });
	playerSheet.addTransitionAnimation(IDLE_RIGHT, RUNNING_LEFT, transition, offset, 4);
	lightSheet.addTransitionAnimation(IDLE_RIGHT, RUNNING_LEFT, transition, offset, 4);


	transition = Animation({ 0, 7, 3, 256, 256, 0 });
	offset = Animation({ 10, 6, 4, 256, 256, 0 });
	playerSheet.addTransitionAnimation(IDLE_LEFT, RUNNING_RIGHT, transition, offset, 4);
	lightSheet.addTransitionAnimation(IDLE_LEFT, RUNNING_RIGHT, transition, offset, 4);

	playerState = IDLE_RIGHT;

	playerSheet.setAnimation(IDLE_RIGHT);
	lightSheet.setAnimation(IDLE_RIGHT);
}

void PlayerObject::handleInputs()
{
	if (!window->hasFocus())
	{
		return;
	}

	playerState = IDLE_RIGHT;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{

		playerState = IDLE_RIGHT;
		facingRight = true;
		return;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		playerState = RUNNING_RIGHT;
		facingRight = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		playerState = RUNNING_LEFT;
		facingRight = false;
	}
}

void PlayerObject::updateSprites()
{
	auto frameTimeDuration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - frameTime);
	float deltaFrameTime = (float)frameTimeDuration.count() / 1000000;

	if ((int)playerState < 0)
	{
		playerState = (PlayerState)((int)facingRight * -1 - 1);
	}

	switch (playerState)
	{
	case IDLE_RIGHT:
	case IDLE_LEFT:
		currentSpeed = 0.0f;
		break;
	case RUNNING_RIGHT:
		if (playerSheet.getAnimationState() != ANIMATION_OFFSET)
		{
			currentSpeed += runSpeed * 6.0f * deltaTime;
			if (currentSpeed > runSpeed)
			{
				currentSpeed = runSpeed;
			}
		}
		else
		{
			currentSpeed = 0;
		}
		x += (float)currentSpeed * deltaTime;
		break;
	case RUNNING_LEFT:
		if (playerSheet.getAnimationState() != ANIMATION_OFFSET)
		{
			currentSpeed += runSpeed * 6.0f * deltaTime; 
			if (currentSpeed > runSpeed)
			{
				currentSpeed = runSpeed;
			}
		}
		else
		{
			currentSpeed = 0;
		}
		x -= (float)currentSpeed * deltaTime;
		break;
	default:
		break;
	}

	playerSheet.setPosition(sf::Vector2f(this->x, this->y));
	lightSheet.setPosition(sf::Vector2f(this->x, this->y));

}

PlayerObject::PlayerObject(float x, float y, float width, float height, std::string playerTexturePath, std::string lightTexturePath)
	:RenderObject(x, y, width, height), spriteWidth(256), spriteHeight(256), spriteX(0), spriteY(0),
	playerState(IDLE_RIGHT), soundMixer(&SoundMixer::getInstance()), fps(32), frameOffset(0),
	playerSheet(16, 16, 32, playerTexturePath),
	lightSheet(16, 16, 32, lightTexturePath)
{
	initializeSpritesheets();
	 
	facingRight = true;

	lightSheet.setBlendMode(sf::BlendAdd);

	float scaleFactor = this->width / 256;

	//frames tracks the frames per row in the spritesheet
	frames = 16;

	playerSheet.setScale(scaleFactor, scaleFactor);
	lightSheet.setScale(scaleFactor, scaleFactor);

	playerSheet.setPosition(sf::Vector2f(x, y));
	lightSheet.setPosition(sf::Vector2f(x, y));

	currentFrame = 0;
	previousFrame = -1;
	runSpeed = fps * 20.0f;
}

void PlayerObject::setFrame(int frame)
{
	currentFrame = ((int)((float)frame / (60.0 / fps))); 

	playerSheet.setFrame(currentFrame, fps);
	lightSheet.setFrame(currentFrame, fps);

	playerSheet.setAnimation(playerState);
	lightSheet.setAnimation(playerState);

	fps60 = frame;
	//std::cout << "current frame:" << currentFrame << std::endl;
	currentFrame += frameOffset;
	currentFrame %= frames;

}

void PlayerObject::render()
{
	update();
	draw();
}

void PlayerObject::update()
{
	currentTime = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - previousTime);

	deltaTime = (float)duration.count() / 1000000;

	previousTime = currentTime;

	handleInputs();
	updateSprites();

	if (currentFrame != previousFrame)
	{
		//called at start of frame
		

		switch (playerState)
		{
		case RUNNING_LEFT:
		case RUNNING_RIGHT:
			if (currentFrame % 8 == 0)
			{
				if (playerSheet.getAnimationState() == ANIMATION_NORMAL)
				{
					std::cout << "step\n";
					auto& sound = soundMixer->addSound("resources/game_files/Sounds/footstep.wav");
					sound.setVolume(1.0f);
				}
			}
			break;
		default:
			break;
		}

		if (previousFrame > currentFrame)
		{
			secondCount++;
		}
		previousFrame = currentFrame;

	}
}

void PlayerObject::draw()
{
	playerSheet.draw(*window);
	lightSheet.draw(*window);
}

void PlayerObject::arrowKeyInput(sf::Keyboard::Key key)
{
}

