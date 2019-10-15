#pragma once
#include"gameObject.h"
#include"components.h"
#include"asteroidslib.h"
#include"object_pool.h"
#include"rocket.h"
#include<set>
#include<iostream>

const float PLAYER_SPEED = 20.0f;

const float	FIRE_TIME_INTERVAL = .2f;

class PlayerBehaviourComponent : public Component
{
	float time_fire_pressed;	// time from the last time the fire button was pressed
	ObjectPool<Rocket> * rockets_pool;
	float velocity = PLAYER_SPEED;
	float acceleration = 0.1;
	float xN, yN, vX, vY;
	float dragFactor = 0.002;
	Mix_Chunk * sound;	
	int channel;

public:
	virtual ~PlayerBehaviourComponent() {}

	virtual void Create(AsteroidsLib* engine, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<Rocket> * rockets_pool)
	{
		Component::Create(engine, go, game_objects);
		this->rockets_pool = rockets_pool;
	}

	virtual void Init()
	{
		go->horizontalPosition = 320;
		go->verticalPosition = 380 - 52;
		go->degrees = 0;

		sound = Mix_LoadWAV("data/thrust.wav");
		if (sound == NULL)
		{
			SDL_Log("Unable to load WAV file: %s\n", Mix_GetError());
		}

		time_fire_pressed = -10000.f;
	}

	virtual void Update(float dt)
	{
		AsteroidsLib::KeyStatus keys;
		engine->getKeyStatus(keys);
		if (keys.right)
		{
			if (go->degrees > 360)
			{
				go->degrees = 0;
			}
			go->degrees += 0.5;
		}
		if (keys.left)
		{
			if (go->degrees == 0)
			{
				go->degrees = 359;
			}
			go->degrees -= 0.5;
		}
		if (keys.up)
		{
			velocity += acceleration;
			channel = Mix_PlayChannel(1, sound, 0);
			if (channel == -1)
			{
				SDL_Log("Unable to play WAV file : %s\n", Mix_GetError());
			}
			MoveUp(-dt * velocity);
		}
		if (!keys.up && velocity >= PLAYER_SPEED)
		{
			velocity -= velocity * dragFactor;
			MoveUp(-dt * velocity);
		}
		if (keys.fire)
		{
			if (CanFire())
			{
				// fetches a rocket from the pool and use it in game_objects
				Rocket * rocket = rockets_pool->FirstAvailable();
				if (rocket != NULL)	// rocket is NULL is the object pool can not provide an object
				{
					rocket->Init(go->horizontalPosition, go->verticalPosition, go->degrees);
					game_objects->insert(rocket);
				}
			}
		}
		if (keys.hyperspace)
		{
			if (CanFire())
			{
				int newX = rand() % 640;
				int newY = rand() % 480;
				go->horizontalPosition = newX;
				go->verticalPosition = newY;
			}
		}
	}


	// param move depends on the time, so the player moves always at the same speed on any computer
	// accelerate the player
	void MoveUp(float move)
	{
		int diffposition;
		bool verticalCheck = false;
		// if the player ship moves off the screen through the right side of the window this function instantiates it again at the appropriate position from the left side
		if (go->horizontalPosition > (640 + 52))
		{
			if ((go->degrees > 0) && (go->degrees <= 90))
			{
				diffposition = 90 - go->degrees;
				go->verticalPosition = go->verticalPosition + (diffposition * 9);
			}
			else if ((go->degrees > 90) && (go->degrees <= 180))
			{
				diffposition = go->degrees - 90;
				go->verticalPosition = go->verticalPosition - (diffposition * 9);
			}
			go->horizontalPosition = go->horizontalPosition - (640 + 52);
		}
		// if the player ship moves off the screen through the left side of the window this function instantiates it again at the appropriate position from the right side	
		else if (go->horizontalPosition < (0 - 52))
		{
			if ((go->degrees < 360) && (go->degrees >= 270))
			{
				diffposition = go->degrees - 270;
				go->verticalPosition = go->verticalPosition + (diffposition * 9);
			}
			else if ((go->degrees >= 180) && (go->degrees < 270))
			{
				diffposition = 270 - go->degrees;
				go->verticalPosition = go->verticalPosition - (diffposition * 9);
			}
			go->horizontalPosition = go->horizontalPosition + (640 + 52);
		}
			
		else if (go->verticalPosition > (480 + 8))
		{
			diffposition = go->horizontalPosition;
			if ((go->degrees > 90) && (go->degrees <= 180))
			{
				diffposition = 640 - (diffposition - 52);
				go->horizontalPosition = diffposition;
			}
			if ((go->degrees > 180) && (go->degrees <= 270))
			{
				diffposition = diffposition - 52;
				go->horizontalPosition = (640/2) + diffposition;
			}
			go->verticalPosition = go->verticalPosition - (480 + 32);
		}
				
		else if (go->verticalPosition < (0 - 52))
		{
			if ((go->degrees > 0) && (go->degrees <= 90))
			{
				diffposition = go->degrees;
				go->horizontalPosition = go->horizontalPosition - (diffposition * 9);
			}
			else if ((go->degrees < 360) && (go->degrees >= 270))
			{
				diffposition = 360 - go->degrees;
				go->horizontalPosition = go->horizontalPosition + (diffposition * 9);
			}
			go->verticalPosition = go->verticalPosition + (480 + 52);
		}
		

		// movement according to the orientation of the plane in space
		go->horizontalPosition -= move * sin(go->degrees * (PI / 180));
		go->verticalPosition += move * cos(go->degrees * (PI / 180));
	}

	// return true if enough time has passed from the previous rocket
	bool CanFire()
	{
		// shoot just if enough time passed by
		if ((engine->getElapsedTime() - time_fire_pressed) < (FIRE_TIME_INTERVAL / game_speed))
			return false;

		time_fire_pressed = engine->getElapsedTime();

		SDL_Log("fire!");
		return true;
	}
};


// the main player
class Player : public GameObject
{
public:

	int lives;	// it's game over when goes below zero 

	virtual ~Player() { SDL_Log("Player::~Player"); }

	virtual void Init()
	{
		SDL_Log("Player::Init");
		GameObject::Init();
		lives = NUM_LIVES;
	}

	virtual void Receive(Message m)
	{
		if (m == HIT)
		{
			SDL_Log("Player::Hit!");
			RemoveLife();

			if (lives <= 0)
				Send(GAME_OVER);
		}
	}

	void RemoveLife()
	{
		lives--;
		SDL_Log("remaining lives %d", lives);
	}
};
