#pragma once

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include"SDL_mixer.h"

class Sprite
{
	SDL_Renderer * renderer;
	SDL_Texture * texture;

public:

	Sprite(SDL_Renderer * renderer, SDL_Texture * texture);

	// Destroys the sprite instance
	void destroy();

	// Draw the sprite at the given position and the angle.
	void draw(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip);
};


class AsteroidsLib
{
public:
	// Destroys the asteroids library instance
	void destroy();

	// Destroys the asteroids library instance and exits
	void quit();

	// Creates the main window. Returns true on success.
	bool init(int width, int height);

	// Clears the screen and draws all sprites and texts which have been drawn
	// since the last update call.
	// If update returns false, the application should terminate.
	void processInput();

	void swapBuffers();

	void clearWindow();

	// Create a sprite given a string.
	Sprite* createSprite(const char* name);

	// Draws the given text.
	void drawText(int x, int y, const char* msg);

	// Plays the required sound
	void playSound(const char* msg);

	// Return the total time spent in the game, in seconds.
	float getElapsedTime();

	struct KeyStatus
	{
		bool fire; // space
		bool left; // left arrow
		bool right; // right arrow
		bool up;	// up arrow
		bool down;	// down arrow
		bool hyperspace;	// left ctrl key
		bool secPlayer;		// key for initiating second player;
		bool esc; // escape button
	};

	// Returns the keyboard status. If a flag is set, the corresponding key is being held down.
	void getKeyStatus(KeyStatus& keys);

private:
	SDL_Window * window;
	SDL_Renderer * renderer;

	TTF_Font* font;

	KeyStatus key;
	double degrees = 0;		//angle for rotation
	SDL_RendererFlip flipType = SDL_FLIP_NONE;		//flip type

	int audio_rate = 22050;
	Uint16 audio_format = AUDIO_S16SYS;
	int audio_channels = 2;
	int audio_buffers = 4096;
};

