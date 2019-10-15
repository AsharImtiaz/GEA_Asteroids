#pragma once
#include"gameObject.h"
#include"asteroidslib.h"
#include"object_pool.h"
#include"player.h"
#include"rocket.h"
#include"bomb.h"
#include"enemyAsteroids.h"
#include"enemySpaceship.h"
#include"EnemySmallerShip.h"
#include"enemyMediumAsteroids.h"
#include"enemySmallerAsteroids.h"
#include<ctime>
#include<set>

class GameManager : public GameObject
{
	std::set<GameObject*> game_objects;	// http://www.cplusplus.com/reference/set/set/

	AsteroidsLib* engine;

	ObjectPool<Rocket> rockets_pool;	// used to instantiate rockets
	ObjectPool<Bomb> bomb_pool;			//used to instantiate bombs for the enemy ships
	ObjectPool<EnemySpaceShip> saucer_pool;		//used to instantiate enemy ships
	ObjectPool<EnemySmallSpaceShip> saucer_small_pool;		//used to instantiate smaller enemy ships
	ObjectPool<Asteroids> asteroids_pool;		//used to instantiate asteroids
	ObjectPool<MediumAsteroids> medium_asteroids_pool;		//used to instantiate the medium asteroids
	ObjectPool<SmallerAsteroids> small_asteroids_pool;		//used to instantiate the smaller asteroids

	Player * player;

	Sprite * life_sprite;		// life sprites for player1

	bool game_over;
	bool check = true;

	unsigned int score = 0;
    int checking;

	int randNum1, randNum2, randNum3, randNum4;

	int asteroidNumber = 0;
	int enemySpaceshipNumber = 0;

public:

	virtual void Create(AsteroidsLib* engine)
	{
		SDL_Log("Game::Create");

		this->engine = engine;

		// Player game object creation
		player = new Player();
		PlayerBehaviourComponent * player_behaviour = new PlayerBehaviourComponent();
		player_behaviour->Create(engine, player, &game_objects, &rockets_pool);
		RenderComponent * player_render = new RenderComponent();
		player_render->Create(engine, player, &game_objects, "data/Shuttle2.png");

		// for checking collision against large asteroid pieces
		CircleCollideComponent * player_collide = new CircleCollideComponent();
		player_collide->Create(engine, player, &game_objects, reinterpret_cast<ObjectPool<GameObject>*>(&asteroids_pool), 26);

		// for checking collision against medium asteroid pieces
		CircleCollideComponent * player_collide_mediumpiece = new CircleCollideComponent();
		player_collide_mediumpiece->Create(engine, player, &game_objects, reinterpret_cast<ObjectPool<GameObject>*>(&medium_asteroids_pool), 13);

		// for checking collision against small asteroid pieces
		CircleCollideComponent * player_collide_smallpiece = new CircleCollideComponent();
		player_collide_smallpiece->Create(engine, player, &game_objects, reinterpret_cast<ObjectPool<GameObject>*>(&small_asteroids_pool), 6);

		// for checking collision against enemy ships
		CircleCollideComponent * player_collide_enemyship = new CircleCollideComponent();
		player_collide_enemyship->Create(engine, player, &game_objects, reinterpret_cast<ObjectPool<GameObject>*>(&saucer_pool), 15);

		// for checking collision against bombs
		CircleCollideComponent * player_collide_bomb = new CircleCollideComponent();
		player_collide_bomb->Create(engine, player, &game_objects, reinterpret_cast<ObjectPool<GameObject>*>(&bomb_pool), 3);

		player->Create();
		player->AddComponent(player_behaviour);
		player->AddComponent(player_render);
		player->AddComponent(player_collide);
		player->AddComponent(player_collide_mediumpiece);
		player->AddComponent(player_collide_smallpiece);
		player->AddComponent(player_collide_enemyship);
		player->AddComponent(player_collide_bomb);
		player->AddReceiver(this);
		game_objects.insert(player);

		// enemy ship's game object object pool creation
		/*saucer_pool.Create(50);
		for (auto saucer = saucer_pool.pool.begin(); saucer != saucer_pool.pool.end(); saucer++)
		{
			EnemySpaceShipBehaviourComponent * saucer_behaviour = new EnemySpaceShipBehaviourComponent();
			saucer_behaviour->Create(engine, *saucer, &game_objects, &bomb_pool);
			RenderComponent * saucer_render = new RenderComponent();
			saucer_render->Create(engine, *saucer, &game_objects, "data/enemyship.png");
			
			// for checking collision against rockets
			CircleCollideComponent * saucer_collide = new CircleCollideComponent();
			saucer_collide->Create(engine, *saucer, &game_objects, reinterpret_cast<ObjectPool<GameObject>*>(&rockets_pool), 32);

			(*saucer)->Create();
			(*saucer)->AddComponent(saucer_behaviour);
			(*saucer)->AddComponent(saucer_render);
			(*saucer)->AddComponent(saucer_collide);
			(*saucer)->AddReceiver(this);
			game_objects.insert(*saucer);
		}*/

		// enemy smaller ship's game object object pool creation
		saucer_small_pool.Create(50);
		for (auto saucer = saucer_pool.pool.begin(); saucer != saucer_pool.pool.end(); saucer++)
		{
			EnemySmallSpaceShipBehaviourComponent * saucer_small_behaviour = new EnemySmallSpaceShipBehaviourComponent();
			saucer_small_behaviour->Create(engine, *saucer, &game_objects, &bomb_pool, player);
			RenderComponent * saucer_small_render = new RenderComponent();
			saucer_small_render->Create(engine, *saucer, &game_objects, "data/enemyship.png");

			// for checking collision against rockets
			CircleCollideComponent * saucer_small_collide = new CircleCollideComponent();
			saucer_small_collide->Create(engine, *saucer, &game_objects, reinterpret_cast<ObjectPool<GameObject>*>(&rockets_pool), 32);

			(*saucer)->Create();
			(*saucer)->AddComponent(saucer_small_behaviour);
			(*saucer)->AddComponent(saucer_small_render);
			(*saucer)->AddComponent(saucer_small_collide);
			(*saucer)->AddReceiver(this);
			game_objects.insert(*saucer);
		}

		// large asteroid's game object object pool creation
		asteroids_pool.Create(50);
		for (auto asteroid = asteroids_pool.pool.begin(); asteroid != asteroids_pool.pool.end(); asteroid++)
		{
			AsteroidBehaviourComponent * asteroids_behaviour = new AsteroidBehaviourComponent();
			asteroids_behaviour->Create(engine, *asteroid, &game_objects, &medium_asteroids_pool);
			RenderComponent * asteroid_render = new RenderComponent();
			asteroid_render->Create(engine, *asteroid, &game_objects, "data/asteroid.png");
			
			// for checking collision against rockets
			CircleCollideComponent * asteroid_collide = new CircleCollideComponent();
			asteroid_collide->Create(engine, *asteroid, &game_objects, reinterpret_cast<ObjectPool<GameObject>*>(&rockets_pool), 100);

			(*asteroid)->Create();
			(*asteroid)->AddComponent(asteroids_behaviour);
			(*asteroid)->AddComponent(asteroid_render);
			(*asteroid)->AddComponent(asteroid_collide);
			(*asteroid)->AddReceiver(this);
			game_objects.insert(*asteroid); 
		}

		// medium asteroid's game object object pool creation
		medium_asteroids_pool.Create(50);
		for (auto asteroid = medium_asteroids_pool.pool.begin(); asteroid != medium_asteroids_pool.pool.end(); asteroid++)
		{
			MediumAsteroidBehaviourComponent *medium_asteroid_behaviour = new MediumAsteroidBehaviourComponent();
			medium_asteroid_behaviour->Create(engine, *asteroid, &game_objects, &small_asteroids_pool);
			RenderComponent * medium_asteroid_render = new RenderComponent();
			medium_asteroid_render->Create(engine, *asteroid, &game_objects, "data/MediumAsteroid.png");

			// for checking collision against rockets
			CircleCollideComponent * medium_asteroid_collide = new CircleCollideComponent();
			medium_asteroid_collide->Create(engine, *asteroid, &game_objects, reinterpret_cast<ObjectPool<GameObject>*>(&rockets_pool), 29);

			(*asteroid)->Create();
			(*asteroid)->AddComponent(medium_asteroid_behaviour);
			(*asteroid)->AddComponent(medium_asteroid_render);
			(*asteroid)->AddComponent(medium_asteroid_collide);
			(*asteroid)->AddReceiver(this);
			game_objects.insert(*asteroid);
		}

		// small asteroid's game object object pool creation
		small_asteroids_pool.Create(50);
		for (auto asteroid = small_asteroids_pool.pool.begin(); asteroid != small_asteroids_pool.pool.end(); asteroid++)
		{
			SmallerAsteroidBehaviourComponent * small_asteroids_behaviour = new SmallerAsteroidBehaviourComponent();
			small_asteroids_behaviour->Create(engine, *asteroid, &game_objects);
			RenderComponent * small_asteroid_render = new RenderComponent();
			small_asteroid_render->Create(engine, *asteroid, &game_objects, "data/smallerAsteroids.png");
			
			// for checking collision against rockets
			CircleCollideComponent * small_asteroid_collide = new CircleCollideComponent();
			small_asteroid_collide->Create(engine, *asteroid, &game_objects, reinterpret_cast<ObjectPool<GameObject>*>(&rockets_pool), 14);

			(*asteroid)->Create();
			(*asteroid)->AddComponent(small_asteroids_behaviour);
			(*asteroid)->AddComponent(small_asteroid_render);
			(*asteroid)->AddComponent(small_asteroid_collide);
			(*asteroid)->AddReceiver(this);
			game_objects.insert(*asteroid);
		}

		// rocket's game object object pool creation
		rockets_pool.Create(30);
		for (auto rocket = rockets_pool.pool.begin(); rocket != rockets_pool.pool.end(); rocket++)
		{
			RocketBehaviourComponent * behaviour = new RocketBehaviourComponent();
			behaviour->Create(engine, *rocket, &game_objects);
			RenderComponent * render = new RenderComponent();
			render->Create(engine, *rocket, &game_objects, "data/rocket.bmp");
			(*rocket)->Create();
			(*rocket)->AddComponent(behaviour);
			(*rocket)->AddComponent(render);
		}

		// bomb's game object object pool creation
		bomb_pool.Create(30);
		for (auto bomb = bomb_pool.pool.begin(); bomb != bomb_pool.pool.end(); bomb++)
		{
			BombBehaviourComponent * behaviour = new BombBehaviourComponent();
			behaviour->Create(engine, *bomb, &game_objects);
			RenderComponent * render = new RenderComponent();
			render->Create(engine, *bomb, &game_objects, "data/bomb.png");
			(*bomb)->Create();
			(*bomb)->AddComponent(behaviour);
			(*bomb)->AddComponent(render);
		}

		life_sprite = engine->createSprite("data/shuttle2.png");

		score = 0;
	}

	virtual void Init()
	{
		srand(std::time(nullptr));
		player->Init();
		saucer_small_pool.pool[0]->Init();
		saucer_small_pool.pool[1]->Init();
		saucer_small_pool.pool[2]->Init();
		saucer_small_pool.pool[3]->Init();
		saucer_small_pool.pool[4]->Init();
		saucer_small_pool.pool[5]->Init();
		saucer_small_pool.pool[6]->Init();
		saucer_small_pool.pool[7]->Init();
		saucer_small_pool.pool[8]->Init();
		enabled = true;
		game_over = false;
	}

	virtual void Update(float dt)
	{
		AsteroidsLib::KeyStatus keys;
		engine->getKeyStatus(keys);
		if (keys.esc) {
			destroy();
			engine->quit();
		}

		// initiate the second player
		if (keys.secPlayer)
		{
			SECOND_PLAYER_CHECK++;
			if (SECOND_PLAYER_CHECK == 1)
			{
				// Init Second Player
			}
		}

		// increase game speed when after every 3000 interval
		if (((score % 3000) == 0) && (score > 0))
		{
			game_speed += 0.01f;
		}

		// interval for spawning the asteroids and asteroids
		randNum1 = rand() % 1500;
		randNum2 = rand() % 1500;
		randNum3 = rand() % 3000;
		randNum4 = rand() % 3000;
		// time to spawn the asteroids
		if (randNum1 == randNum2)
		{
			spawnAsteroids();
		}
		// time to spawn the enemy ships
		else if (randNum3 == randNum4)
		{
			//spawnEnemySpaceships();
		}
		
		if (IsGameOver())
			dt = 0.f;

		for (auto go = game_objects.begin(); go != game_objects.end(); go++)
			(*go)->Update(dt);
		
		//SDL_Delay(10000);
	}

	virtual void Draw()
	{
		char msg[1024];
		sprintf_s(msg, "%07d", Score());
		engine->drawText(10,10,msg);
		sprintf_s(msg, "00");
		engine->drawText(300, 10, msg);
		for (int i = 0; i < player->lives; i++)
			life_sprite->draw(i * 36 + 10, 40, NULL, 0, NULL, SDL_FLIP_NONE);
		sprintf_s(msg, "@1979  ATARI INC.");
		engine->drawText(200, 460, msg);

		/*if (SECOND_PLAYER_CHECK == 0)
		{
			sprintf_s(msg, "PRESS P FOR SECOND PLAYER...");
		}
		if (SECOND_PLAYER_CHECK > 0)
		{
	
		}*/
		
		engine->swapBuffers();
		engine->clearWindow();
	}

	virtual void Receive(Message m)
	{
		if (m == GAME_OVER)
			game_over = true;

		if (m == ASTEROID_DESTROY || m == SAUCER_DESTROY)
			score += POINTS_PER_ASTEROID;

		//higher score for destroying medium asteroids
		if (m == MEDAATEROID_DESTROY)
			score += POINTS_PER_MEDIUM_ASTEROID;

		//even higher score for destroying small asteroids
		if (m == SMASTEROID_DESTROY)
			score += POINTS_PER_SMALL_ASTEROID;
	}


	bool IsGameOver()
	{
		return game_over;
	}

	unsigned int Score()
	{
		return score;
	}

	// function for spawning the asteroids
	void spawnAsteroids()
	{
		asteroids_pool.FirstAvailable()->Init();
		++asteroidNumber;
	}

	// function for spawning the enemy ships
	void spawnEnemySpaceships()
	{
		saucer_pool.FirstAvailable()->Init();
		++enemySpaceshipNumber;
	}

	virtual void destroy()
	{
		SDL_Log("Game::Destroy");

		for (auto go = game_objects.begin(); go != game_objects.end(); go++)
			(*go)->Destroy();

		life_sprite->destroy();

		saucer_pool.Destroy();
		asteroids_pool.Destroy();
		medium_asteroids_pool.Destroy();
		small_asteroids_pool.Destroy();
		rockets_pool.Destroy();
		delete player;
	}
};
