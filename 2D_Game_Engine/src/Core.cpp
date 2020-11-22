#include "Core.h"
#include "TextureHandler.h"
#include "ECS/Components.h"
#include "Vector2.h"

SDL_Renderer* Core::gRenderer = nullptr;
//Event handler
SDL_Event Core::event;

Manager manager;

auto& player(manager.addEntity());

Core::Core(){}

Core::~Core(){}

void Core::init(const char *title, int width, int height)
{
	// Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		printf("SDL subsystems Initialized!\n");

		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!\n");
		}

		//Create window
		gWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			printf("Window created!\n");

			//Create renderer for window with V-sync
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{			
				printf("Renderer created!\n");

				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}
	isRunning = success;

	//ECS
	player.addComponent<TransformComponent>();
	player.addComponent<SpriteComponent>("assets/dot.bmp");
	player.addComponent<KeyboardController>();
}


void Core::handleEvents()
{
	
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
}

void Core::update()
{
	//Update entities
	manager.refresh();
	manager.update();	
}

void Core::render()
{
	//Clear screen
	SDL_RenderClear(gRenderer);
	manager.draw();
	//Update screen
	SDL_RenderPresent(gRenderer);
}

void Core::close()
{
	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	//Destroy window and renderer
	SDL_DestroyWindow(gWindow);
	SDL_DestroyRenderer(gRenderer);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool Core::running()
{
	return isRunning;
}
