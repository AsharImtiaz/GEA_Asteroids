#pragma once

#include <vector>
#include "SDL.h"
#include "SDL_ttf.h"

enum Message { HIT, ASTEROID_DESTROY, MEDAATEROID_DESTROY, SMASTEROID_DESTROY, GAME_OVER, LEVEL_WIN, NO_MSG, QUIT, SAUCER_DESTROY };

class Component;  

class GameObject
{
protected:
	std::vector<GameObject*> receivers;
	std::vector<Component*> components;

	std::string id;

public:
	double horizontalPosition;
	double verticalPosition;
	double degrees = 0;		// for checking orientation in screen
	bool breakCheck = false;		// check for when asteroid is to be broken
	bool enabled;

	virtual ~GameObject();

	virtual void Create();
	virtual void AddComponent(Component * component);

	virtual void Init();
	virtual void Update(float dt);
	virtual void Destroy();
	virtual void AddReceiver(GameObject *go);
	virtual void Receive(Message m) {}
	void Send(Message m);

	template<typename T>
	T GetComponent() {
		for (Component * c : components) {
			T t = dynamic_cast<T>(c);  
			if (t != nullptr) {
				return t;
			}
		}

		return nullptr;
	}

	std::string getID() {
		return id;
	}
};
