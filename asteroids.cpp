float game_speed = 1.f;
const float PI = 3.14159;
const unsigned int	POINTS_PER_ASTEROID = 100;
const unsigned int	POINTS_PER_MEDIUM_ASTEROID = 150;
const unsigned int	POINTS_PER_SMALL_ASTEROID = 200;
const unsigned int	NUM_LIVES = 3;
int MEDIUM_ASTEROID_COUNT = 0;
int MEDIUM_ASTEROID_DIRECTION = 0;
int SMALL_ASTEROID_COUNT = 0;
int SMALL_ASTEROID_DIRECTION = 0;
int BOMB_DIRECTION = 0;
int PLAYER_TRACKER_X = 0;
int PLAYER_TRACKER_Y = 0;
int SECOND_PLAYER_CHECK = 0;

#include"asteroidslib.h"
#include"gameManager.h"
#include<iostream>
int main(int argc, char** argv)
{
	AsteroidsLib engine;
	engine.init(640,480);

	GameManager game;
	game.Create(&engine);
	game.Init();

	float lastTime = engine.getElapsedTime();
	while (true)
	{
		float newTime = engine.getElapsedTime();
		float dt = newTime - lastTime;
		dt = dt * game_speed;
		lastTime = newTime;

		game.Draw();
		engine.processInput();
		game.Update(dt);	
	}
	return 0;
}