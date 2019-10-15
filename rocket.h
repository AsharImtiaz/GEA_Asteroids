#pragma once
#include"gameObject.h"
#include"components.h"
#include"asteroidslib.h"

const float ROCKET_SPEED = 340.0f;

class RocketBehaviourComponent : public Component
{
public:

	void Update(float dt)
	{
		go->horizontalPosition += dt * ROCKET_SPEED * sin(go->degrees * (PI / 180));
		go->verticalPosition -= dt * ROCKET_SPEED * cos(go->degrees * (PI / 180));

		if (go->verticalPosition < -10 || go->verticalPosition > 490 || go->horizontalPosition < -10 || go->horizontalPosition > 650) // When the rocket goes out of bounds of the screen, it disappears.
			go->enabled = false;
	}
};


// rockets are shot by the player towards the incoming asteroids or enemy ships
class Rocket : public GameObject
{

public:

	virtual void Init(double xPos, double yPos, double angle)
	{
		SDL_Log("Rocket::Init");
		GameObject::Init();

		horizontalPosition = xPos;
		verticalPosition = yPos;
		degrees = angle;

		Mix_Chunk * sound = Mix_LoadWAV("data/Sounds/fire.wav");		//where to free sound?!
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
			SDL_Log("Rocket::Hit");
		}
	}
};