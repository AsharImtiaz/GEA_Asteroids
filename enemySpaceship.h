#pragma once
#include<iostream>
#include"gameObject.h"
#include"components.h"
#include"asteroidslib.h"
#include"enemySmallerAsteroids.h"
#include<cstdlib>
#include<ctime>
#include<set>
#include<math.h>

const float	SAUCER_SPEED = 50.0f;
const float BOMB_FIRE_TIME_INTERVAL = .5f;


int changeDirection()
{
	int random = rand() % 8;
	return random;
};

class EnemySpaceShipBehaviourComponent : public Component
{
	int rand1, rand2, rand3, rand4;			// random variables to decide when to change direction and when to fire bullet
	double xPos, yPos;
	float time_fire_pressed;	// time from the last time the fire button was pressed
	ObjectPool<Bomb> * bomb_pool;

public:
	virtual ~EnemySpaceShipBehaviourComponent() {}

	virtual void Create(AsteroidsLib* engine, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<Bomb> * bombs_pool)
	{
		Component::Create(engine, go, game_objects);
		this->bomb_pool = bombs_pool;
	}

	virtual void Init(int x, int y)
	{
		time_fire_pressed = -10000.f;
	}

	virtual void Update(float dt)
	{
		rand1 = rand() % 2000;
		rand2 = rand() % 2000;
		// time to change direction
		if (rand1 == rand2)
		{
			go->degrees = changeDirection();
		}
		Move(dt * SAUCER_SPEED * game_speed);
		rand3 = rand() % 5;
		rand4 = rand() % 5;
		// time to fire bomb from Enemy Space ship
		if ((CanFire()) && (rand3 == rand4))
		{
			// fetches a bomb from the pool and use it in game_objects
			Bomb * bomb = bomb_pool->FirstAvailable();
			if (bomb != NULL)	// bomb is NULL is the object pool can not provide an objec
			{
				//bomb->Init(go->horizontalPosition, go->verticalPosition, go->degrees);
				//game_objects->insert(bomb);
			}
		}
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

	bool CanFire()
	{
		// shoot just if enough time passed by
		if ((engine->getElapsedTime() - time_fire_pressed) < (BOMB_FIRE_TIME_INTERVAL / game_speed))
			return false;

		time_fire_pressed = engine->getElapsedTime();

		SDL_Log("Bomb fire!");
		return true;
	}
};


// the enemy spaceship
class EnemySpaceShip : public GameObject
{
public:
	Mix_Chunk * sound = Mix_LoadWAV("data/Sounds/bangMedium.wav");
	int channel;

	virtual ~EnemySpaceShip() { SDL_Log("EnemySpaceShip::~EnemySpaceShip"); }

	virtual void Init()
	{
		int randNum1;		//first randNumber to decide side (up, down, left, right)
		int randNum2;		//second randNumber to decide random position of origin from either sides

		SDL_Log("Enemy::Init");
		GameObject::Init();

		if (sound == NULL)
		{
			SDL_Log("Unable to load WAV file: %s\n", Mix_GetError());
		}

		randNum1 = changeDirection();

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
			SDL_Log("EnemySpaceShip::Hit!");

			channel = Mix_PlayChannel(-1, sound, 0);
			if (channel == -1)
			{
				SDL_Log("Unable to play WAV file : %s\n", Mix_GetError());
			}

			enabled = false;
			Send(SAUCER_DESTROY);
		}
	}
};
