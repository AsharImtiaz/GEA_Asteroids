#pragma once
#include<iostream>
#include<cstdlib>
#include<ctime>
#include"gameObject.h"
#include"components.h"
#include"asteroidslib.h"

const float SMALLER_ASTEROID_SPEED = 80.0f;

class SmallerAsteroidBehaviourComponent : public Component
{
public:

	virtual void Update(float dt)
	{
		// When the smaller asteroid pieces gooes out of bounds of the small screen, it disappears.
		if (go->verticalPosition < -10 || go->verticalPosition > 490 || go->horizontalPosition < -10 || go->horizontalPosition > 650)
		{
			go->enabled = false;
			//delete this;
		}
			
		// moving top from bottom
		if (go->degrees == 0)
		{
			go->verticalPosition += SMALLER_ASTEROID_SPEED * dt * game_speed;
		}
		// moving from right to left
		else if (go->degrees == 1)
		{
			go->horizontalPosition -= SMALLER_ASTEROID_SPEED * dt * game_speed;
		}
		// moving from bottom to top
		else if (go->degrees == 2)
		{
			go->verticalPosition -= SMALLER_ASTEROID_SPEED * dt * game_speed;
		}
		// moving from left to right
		else if (go->degrees == 3)
		{
			go->horizontalPosition += SMALLER_ASTEROID_SPEED * dt * game_speed;
		}
		else
		{
		}
	}
};

// smaller asteroid particles disinegrated from the bigger asteroids
class SmallerAsteroids : public GameObject
{
public:
	virtual void Init(double xPos, double yPos)
	{
		SDL_Log("Smaller Asteroids::Init");
		GameObject::Init();

		horizontalPosition = xPos;
		verticalPosition = yPos;
		//keeping the limit with the 4 directions (up, down, left, right)
		if (SMALL_ASTEROID_DIRECTION == 4)
		{
			SMALL_ASTEROID_DIRECTION = 0;	
		}
		degrees = SMALL_ASTEROID_DIRECTION;		//used to determine direction of asteroid piece
		SMALL_ASTEROID_DIRECTION++;		//switch direction for next asteroid pience
	}

	virtual void Receive(Message m)
	{
		if (!enabled)
			return;

		if (m == HIT)
		{
			enabled = false;
			SDL_Log("Rocket::Hit");
			Send(SMASTEROID_DESTROY); 
		}
	}
};