#pragma once
#include"gameObject.h"
#include"components.h"
#include"asteroidslib.h"

const float BOMB_SPEED = 300.0f;

class BombBehaviourComponent : public Component
{
public:

	void Update(float dt)
	{
		if (go->verticalPosition < -10 || go->verticalPosition > 490 || go->horizontalPosition < -10 || go->horizontalPosition > 650) // When the bomb goes out of bounds of the screen, it disappears.
			go->enabled = false;

		go->horizontalPosition = (go->horizontalPosition / go->degrees) * BOMB_SPEED * dt;
		go->verticalPosition = (go->verticalPosition / go->degrees) * BOMB_SPEED * dt;

		/*
		// moving from top to bottom
		if (go->degrees == 0)
		{
			go->verticalPosition += BOMB_SPEED * dt;
		}
		// moving from right to left
		else if (go->degrees == 1)
		{
			go->horizontalPosition -= BOMB_SPEED * dt;
		}
		// moving from bottom to top
		else if (go->degrees == 2)
		{
			go->verticalPosition -= BOMB_SPEED * dt;
		}
		// moving from left to right
		else if (go->degrees == 3)
		{
			go->horizontalPosition += BOMB_SPEED * dt;
		}
		else
		{
		}
		*/
	}
};


// rockets are shot by the enemy ship 
class Bomb : public GameObject
{
	double tempX, tempY;
public:

	virtual void Init(double xPos, double yPos, double degrees, double pxPos, double pyPos)
	{
		SDL_Log("Bomb::Init");
		GameObject::Init();

		horizontalPosition = xPos;
		verticalPosition = yPos;

		tempX = horizontalPosition - pxPos;
		tempY = verticalPosition - pyPos;

		degrees = std::sqrt((tempX * tempX) + (tempY * tempY));
		/*
		// four possible directions available so limiting the numbers
		if (BOMB_DIRECTION == 4)
		{
			BOMB_DIRECTION = 0;
		}
		degrees = BOMB_DIRECTION;
		BOMB_DIRECTION++;		// change direction each time so same direction is not repeated twice

		*/
		Mix_Chunk * sound = Mix_LoadWAV("data/Sounds/fire.wav");		
		if (sound == NULL)
		{
			SDL_Log("Unable to load WAV file: %s\n", Mix_GetError());
		}

		int channel = Mix_PlayChannel(-1, sound, 0);
		if (channel == -1)
		{
			SDL_Log("Unable to play WAV file : %s\n", Mix_GetError());
		}
	}

	virtual void Receive(Message m)
	{
		if (!enabled)
			return;

		if (m == HIT)
		{
			enabled = false;
			SDL_Log("Bomb::Hit");
		}
	}
};