#include "components.h"
#include "gameObject.h"
#include "asteroidslib.h"

void Component::Create(AsteroidsLib * engine, GameObject * go, std::set<GameObject*>* game_objects)
{
	this->go = go;
	this->engine = engine;
	this->game_objects = game_objects;
}

void RenderComponent::Create(AsteroidsLib * engine, GameObject * go, std::set<GameObject*>* game_objects, const char * sprite_name)
{
	Component::Create(engine, go, game_objects);

	sprite = engine->createSprite(sprite_name);
}

void RenderComponent::Update(float dt)
{
	if (!go->enabled)
		return;
	
	if (sprite)
		sprite->draw(int(go->horizontalPosition), int(go->verticalPosition), NULL, go->degrees, NULL, SDL_FLIP_NONE);
}

void RenderComponent::Destroy()
{
	if (sprite != NULL)
		sprite->destroy();
	sprite = NULL;
}


void CollideComponent::Create(AsteroidsLib* engine, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<GameObject> * coll_objects)
{
	Component::Create(engine, go, game_objects);
	this->coll_objects = coll_objects;
}


void CollideComponent::Update(float dt)
{
	for (auto i = 0; i < coll_objects->pool.size(); i++)
	{
		GameObject * go0 = coll_objects->pool[i];
		if (go0->enabled)
		{
			if ((go0->horizontalPosition > go->horizontalPosition - 55) &&
				(go0->horizontalPosition < go->horizontalPosition + 55) &&
				(go0->verticalPosition > go->verticalPosition - 52) &&
				(go0->verticalPosition < go->verticalPosition + 52))
			{
				go->Receive(HIT);
				go0->Receive(HIT);
			}
		}
	}
}

// implementation of Circular Bounding Volumes
void CircleCollideComponent::Create(AsteroidsLib* engine, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<GameObject> * coll_objects, double radius)
{
	Component::Create(engine, go, game_objects);
	this->coll_objects = coll_objects;
	this->radius = radius;
}


void CircleCollideComponent::Update(double dt)
{
	for (auto i = 0; i < coll_objects->pool.size(); i++)
	{
		GameObject * go0 = coll_objects->pool[i];
		if (go0 != go && go0->enabled)
		{
			CircleCollideComponent* otherCollider = go0->GetComponent<CircleCollideComponent*>();
			if (otherCollider != nullptr) { //if the other GameObject doesn't have a CircleColliderComponent we shouldn't go in here...
				bool intersection;
				int sumofRadius = otherCollider->radius + this->radius;
				int distanceBetweenCenters = sqrt((pow((go0->horizontalPosition - go->horizontalPosition), 2)) + (pow((go0->verticalPosition - go->verticalPosition), 2)));
				if (distanceBetweenCenters <= sumofRadius)
				{
					intersection = true;
				}
				else
				{
					intersection = false;
				}
				if (intersection) 
				{
					go->Receive(HIT);
					go0->Receive(HIT);
				}
			}
		}
	}
}