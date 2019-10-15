#pragma once
#include<iostream>
#include"gameObject.h"
#include"components.h"
#include"asteroidslib.h"
#include"enemyMediumAsteroids.h"
#include"enemySmallerAsteroids.h"
#include<cstdlib>
#include<ctime>
#include<set>
#include<math.h>

const float	ASTEROID_SPEED = 50.0f;


class AsteroidBehaviourComponent : public Component
{
	double xPos, yPos;
	float time_fire_pressed;	// time from the last time the fire button was pressed
	ObjectPool<MediumAsteroids> * medium_pool;

public:
	virtual ~AsteroidBehaviourComponent() {}

	virtual void Create(AsteroidsLib* engine, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<MediumAsteroids> * medium_pool)
	{
		Component::Create(engine, go, game_objects);
		this->medium_pool = medium_pool;
	}

	virtual void Init(int x, int y)
	{
		time_fire_pressed = -10000.f;
	}

	virtual void Update(float dt)
	{
		Move(dt * ASTEROID_SPEED * game_speed);
		// check to initiate the smaller asteroid pieces when the current asteroid piece is destroyed
		if (go->breakCheck == true)
		{
			if (medium_pool->pool[MEDIUM_ASTEROID_COUNT] != NULL)
			{
				xPos = go->horizontalPosition;
				yPos = go->verticalPosition;

				medium_pool->FirstAvailable()->Init(xPos, yPos);
			}
			MEDIUM_ASTEROID_COUNT++;
			if (medium_pool->pool[MEDIUM_ASTEROID_COUNT] != NULL)
			{
				xPos = go->horizontalPosition;
				yPos = go->verticalPosition;

				medium_pool->FirstAvailable()->Init(xPos, yPos);
			}
			MEDIUM_ASTEROID_COUNT++;
			go->enabled = false;
		}
		go->breakCheck = false;
	}

	void Move(float move)
	{
		// When the asteroid goes out of bounds, disable it.
		if (go->verticalPosition < -10 || go->verticalPosition > 490 || go->horizontalPosition < -10 || go->horizontalPosition > 650)
			go->enabled = false;

		if (go->degrees == 0)
		{
			go->verticalPosition += move;
			go->horizontalPosition += move / 3;
		}
		if (go->degrees == 1)
		{
			go->verticalPosition += move;
			go->horizontalPosition -= move / 3;
		}
		if (go->degrees == 2)
		{
			go->verticalPosition += move / 3;
			go->horizontalPosition -= move;
		}
		if (go->degrees == 3)
		{
			go->verticalPosition -= move / 3;
			go->horizontalPosition -= move;
		}
		if (go->degrees == 4)
		{
			go->verticalPosition -= move;
			go->horizontalPosition += move / 3;
		}
		if (go->degrees == 5)
		{
			go->verticalPosition -= move;
			go->horizontalPosition -= move / 3;
		}
		if (go->degrees == 6)
		{
			go->verticalPosition += move / 3;
			go->horizontalPosition += move;
		}
		if (go->degrees == 7)
		{
			go->verticalPosition -= move / 3;
			go->horizontalPosition += move;
		}
	}
};


// the asteroid
class Asteroids : public GameObject
{
public:
	Mix_Chunk * sound = Mix_LoadWAV("data/Sounds/bangMedium.wav");
	int channel;

	virtual ~Asteroids() { SDL_Log("Asteroids::~Asteroids"); }

	virtual void Init()
	{
		int randNum1;		//first randNumber to decide side (up, down, left, right)
		int randNum2;		//second randNumber to decide random position of origin from either sides

		SDL_Log("Enemy::Init");
		GameObject::Init();
		
		
		randNum1 = rand() % 8;
		
		//orgin from above and moving to right diagonal
		if (randNum1 == 0)
		{
			randNum2 = rand() % 640;
			std::cout << "Up side and Pos is " << randNum2 << std::endl;
			horizontalPosition = randNum2;
			verticalPosition = 0;
			degrees = 0;
		}
		//orgin from above and moving to left diagonal
		else if (randNum1 == 1)
		{
			randNum2 = rand() % 640;
			std::cout << "Up side and Pos is " << randNum2 << std::endl;
			horizontalPosition = randNum2;
			verticalPosition = 0;
			degrees = 1;
		}
		//orgin from right and moving to top diagonal
		else if (randNum1 == 2)
		{
			randNum2 = rand() % 480;
			std::cout << "Right side and Pos is " << randNum2 << std::endl;
			horizontalPosition = 650;
			verticalPosition = randNum2;
			degrees = 2;
		}
		//orgin from right and moving to bottom diagonal
		else if (randNum1 == 3)
		{
			randNum2 = rand() % 480;
			std::cout << "Right side and Pos is " << randNum2 << std::endl;
			horizontalPosition = 650;
			verticalPosition = randNum2;
			degrees = 3;
		}
		//orgin from bottom and moving to right diagonal
		else if (randNum1 == 4)
		{
			randNum2 = rand() % 640;
			std::cout << "Down side and Pos is " << randNum2 << std::endl;
			horizontalPosition = randNum2;
			verticalPosition = 490;
			degrees = 4;
		}
		//orgin from bottom and moving to left diagonal
		else if (randNum1 == 5)
		{
			randNum2 = rand() % 640;
			std::cout << "Down side and Pos is " << randNum2 << std::endl;
			horizontalPosition = randNum2;
			verticalPosition = 490;
			degrees = 5;
		}
		//orgin from left and moving to top diagonal
		else if (randNum1 == 6)
		{
			randNum2 = rand() % 480;
			std::cout << "Left side and Pos is " << randNum2 << std::endl;
			horizontalPosition = 0;
			verticalPosition = randNum2;
			degrees = 6;
		}
		//orgin from left and moving to bottom diagonal
		else if (randNum1 == 7)
		{
			randNum2 = rand() % 480;
			std::cout << "Left side and Pos is " << randNum2 << std::endl;
			horizontalPosition = 0;
			verticalPosition = randNum2;
			degrees = 7;
		}
		else
		{
		}
	}

	virtual void Receive(Message m)
	{
		if (m == HIT)
		{
			SDL_Log("Enemy::Hit!");

			channel = Mix_PlayChannel(-1, sound, 0);
			if (channel == -1)
			{
				SDL_Log("Unable to play WAV file : %s\n", Mix_GetError());
			}

			breakCheck = true;		//check to indicate that current asteroid piece has been destroyed
			Send(ASTEROID_DESTROY);
		}
	}
};
