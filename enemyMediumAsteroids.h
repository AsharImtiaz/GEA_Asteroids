#pragma once
#include<iostream>
#include<cstdlib>
#include<ctime>
#include"enemySmallerAsteroids.h"
#include"gameObject.h"
#include"components.h"
#include"asteroidslib.h"


const float MEDIUM_ASTEROID_SPEED = 80.0f;

class MediumAsteroidBehaviourComponent : public Component
{
	double xPos, yPos;
	ObjectPool<SmallerAsteroids> * small_pool;

public:
	virtual ~MediumAsteroidBehaviourComponent() {}
	
	virtual void Create(AsteroidsLib* engine, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<SmallerAsteroids> * small_pool)
	{
		Component::Create(engine, go, game_objects);
		this->small_pool = small_pool;
	}

	virtual void Update(float dt)
	{
		Move(dt * MEDIUM_ASTEROID_SPEED * game_speed);
		if (go->breakCheck == true)
		{
			if (small_pool->pool[SMALL_ASTEROID_COUNT] != NULL)
			{
				xPos = go->horizontalPosition;
				yPos = go->verticalPosition;

				small_pool->FirstAvailable()->Init(xPos, yPos);
			}
			SMALL_ASTEROID_COUNT++;
			if (small_pool->pool[SMALL_ASTEROID_COUNT] != NULL)
			{
				xPos = go->horizontalPosition;
				yPos = go->verticalPosition;

				small_pool->FirstAvailable()->Init(xPos, yPos);
			}
			SMALL_ASTEROID_COUNT++;
			go->enabled = false;
		}
		go->breakCheck = false;
	}

	void Move(float move)
	{
		// When the medium asteroid pieces gooes out of bounds of the small screen, it disappears.
		if (go->verticalPosition < -10 || go->verticalPosition > 490 || go->horizontalPosition < -10 || go->horizontalPosition > 650)
		{
			go->enabled = false;
		}

		if (go->degrees == 0)
		{
			go->verticalPosition += move;
			go->horizontalPosition += move;
		}
		if (go->degrees == 1)
		{
			go->verticalPosition += move;
			go->horizontalPosition -= move;
		}
		if (go->degrees == 2)
		{
			go->verticalPosition += move;
			go->horizontalPosition -= move;
		}
		if (go->degrees == 3)
		{
			go->verticalPosition -= move;
			go->horizontalPosition -= move;
		}
		if (go->degrees == 4)
		{
			go->verticalPosition -= move;
			go->horizontalPosition += move;
		}
		if (go->degrees == 5)
		{
			go->verticalPosition -= move;
			go->horizontalPosition -= move;
		}
		if (go->degrees == 6)
		{
			go->verticalPosition += move;
			go->horizontalPosition += move;
		}
		if (go->degrees == 7)
		{
			go->verticalPosition -= move;
			go->horizontalPosition += move;
		}
	}
};

// medium asteroid particles disinegrated from the bigger asteroids
class MediumAsteroids : public GameObject
{
public:
	Mix_Chunk * sound = Mix_LoadWAV("data/Sounds/bangMedium.wav");
	int channel;

	virtual void Init(double xPos, double yPos)
	{
		SDL_Log("Medium Asteroids::Init");
		GameObject::Init();

		horizontalPosition = xPos;
		verticalPosition = yPos;
		//keeping the limit with 8 directions (all possible diagonals)
		if (MEDIUM_ASTEROID_DIRECTION == 8)
		{
			MEDIUM_ASTEROID_DIRECTION = 0;
		}
		degrees = MEDIUM_ASTEROID_DIRECTION;		//used to determine direction of asteroid piece
		MEDIUM_ASTEROID_DIRECTION++;		//switch direction for next asteroid pience
	}

	virtual void Receive(Message m)
	{
		if (!enabled)
			return;

		if (m == HIT)
		{
			SDL_Log("Rocket::Hit");

			channel = Mix_PlayChannel(-1, sound, 0);
			if (channel == -1)
			{
				SDL_Log("Unable to play WAV file : %s\n", Mix_GetError());
			}

			breakCheck = true;
			Send(MEDAATEROID_DESTROY);
		}
	}
};