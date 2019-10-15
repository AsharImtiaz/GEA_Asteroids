#include "asteroidslib.h"

// Creates the main window. Returns true on success.
bool AsteroidsLib::init(int width, int height)
{
	SDL_Log("Initializing the engine...\n");

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL failed the initialization: %s\n", SDL_GetError());
		return false;
	}

	//Create window
	window = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	
	//Create renderer for window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	TTF_Init();
	font = TTF_OpenFont("data/Hyperspace.ttf", 16); //this opens a font style and sets a size
	if (font == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "font cannot be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// Initializing SDL image with PNG images
	IMG_Init(IMG_INIT_PNG);

	// Initialize SDL_mixer
	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize audio: %s\n", Mix_GetError()); 
	}

	// initialize the keys
	key.secPlayer = false;  key.hyperspace = false; key.fire = false;	key.left = false;	key.right = false, key.esc = false, key.up = false, key.down = false;

	//Initialize renderer color
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

	//Clear screen
	SDL_RenderClear(renderer);

	SDL_Log("Engine up and running...\n");
	return true;
}


// Destroys the asteroids library instance
void AsteroidsLib::destroy()
{
	SDL_Log("Shutting down the engine\n");

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	TTF_CloseFont(font);

	TTF_Quit();
	IMG_Quit();
	Mix_CloseAudio();
	SDL_Quit();
}

void AsteroidsLib::quit() {
	destroy();
	exit(0);
}


void AsteroidsLib::processInput()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
			case SDLK_q:
				key.esc = true;
				break;
			case SDLK_SPACE:
				key.fire = true;
				break;
			case SDLK_LEFT:
				key.left = true;
				break;
			case SDLK_RIGHT:
				key.right = true;
				break;
			case SDLK_UP:
				key.up = true;
				break;
			case SDLK_DOWN:
				key.down = true;
				break;
			case SDLK_LCTRL:
				key.hyperspace = true;
				break;
			case SDLK_p:
				key.secPlayer = true;
				break;
			}
		}

		if (event.type == SDL_KEYUP)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_UP:
				key.up = false;
				break;
			case SDLK_SPACE:
				key.fire = false;
				break;
			case SDLK_LEFT:
				key.left = false;
				break;
			case SDLK_RIGHT:
				key.right = false;
				break;
			case SDLK_DOWN:
				key.down = false;
				break;
			case SDLK_LCTRL:
				key.hyperspace = false;
				break;
			case SDLK_p:
				key.secPlayer = false;
				break;
			}
		}
	}
}

void AsteroidsLib::swapBuffers() {
	//Update screen
	SDL_RenderPresent(renderer);
}

void AsteroidsLib::clearWindow() {
	//Clear screen
	SDL_RenderClear(renderer);
}


Sprite * AsteroidsLib::createSprite(const char * path)
{
	SDL_Texture * texture;
	SDL_Surface* surf = IMG_Load(path);
	if (surf == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load image %s! SDL_image Error: %s\n", path, SDL_GetError());
		return NULL;
	}

	//Create texture from surface pixels
	texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
		return NULL;
	}

	//Get rid of old loaded surface
	SDL_FreeSurface(surf);

	Sprite * sprite = new Sprite(renderer, texture);

	return sprite;
}

void AsteroidsLib::drawText(int x, int y, const char * msg)
{
	SDL_Color white = { 255, 255, 255 };  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

	SDL_Surface* surf = TTF_RenderText_Solid(font, msg, white); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

	SDL_Texture* msg_texture = SDL_CreateTextureFromSurface(renderer, surf); //now you can convert it into a texture

	int w = 0;
	int h = 0;
	SDL_QueryTexture(msg_texture, NULL, NULL, &w, &h);
	SDL_Rect dst_rect = { x, y, w, h };

	SDL_RenderCopy(renderer, msg_texture, NULL, &dst_rect);

	SDL_DestroyTexture(msg_texture);
	SDL_FreeSurface(surf);
}

// function for playing different sounds
void AsteroidsLib::playSound(const char * path)
{
	Mix_Chunk * sound = Mix_LoadWAV(path);
	// if sound NULL check error
	if (sound == NULL)
	{
		SDL_Log("Unable to load WAV file: %s\n", Mix_GetError());
	}
	int channel;
	channel = Mix_PlayChannel(-1, sound, 0);
	if (channel == -1)
	{
		SDL_Log("Unable to play WAV file : %s\n", Mix_GetError());
	}

	Mix_FreeChunk(sound);
}

float AsteroidsLib::getElapsedTime()
{
	return SDL_GetTicks() / 1000.f;
}

void AsteroidsLib::getKeyStatus(KeyStatus & keys)
{
	keys.fire = key.fire;
	keys.left = key.left;
	keys.right = key.right;
	keys.up = key.up;
	keys.esc = key.esc;
	keys.hyperspace = key.hyperspace;
	keys.secPlayer = key.secPlayer;
}


Sprite::Sprite(SDL_Renderer * renderer, SDL_Texture * texture)
{
	this->renderer = renderer;
	this->texture = texture;
}


void Sprite::draw(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Rect rect;
	SDL_Point midpoint;		//to set the angle of rotation to the centre of sprite (by default it is set at the top left corner of the sprite)

	rect.x = x;
	rect.y = y;
	rect.w = 35;
	rect.h = 35;

	midpoint.x = rect.w / 2;		
	midpoint.y = rect.h / 2;

	SDL_QueryTexture(texture, NULL, NULL, &(rect.w), &(rect.h));

	//Render texture to screen
	SDL_RenderCopyEx(renderer, texture, clip, &rect, angle, &midpoint, flip);
}

void Sprite::destroy()
{
	SDL_DestroyTexture(texture);
}


