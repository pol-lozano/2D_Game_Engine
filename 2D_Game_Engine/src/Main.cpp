//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <fstream>

#include "Sprite.h"
#include "Timer.h"
#include "Entity.h"
#include "Tile.h"


// Constants
const char* GAME_TITLE = "Test";
//Camera size
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
//Level size
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;
//Fps settings
const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

//Scene sprites
Sprite gTest;
Sprite gTimeTextTexture;
Sprite gDotTexture;
Sprite gTileTexture;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia(Tile* tiles[]);

//Frees media and shuts down SDL
void close(Tile* tiles[]);

//Sets tiles from tile map
bool setTiles(Tile* tiles[]);

//The window we'll be rendering to
SDL_Window* gWindow;

//The window renderer
SDL_Renderer* gRenderer;

//Globally used font
TTF_Font* gFont;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow(GAME_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window with V-sync
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Set static reference to global renderer
				Sprite::m_Renderer = gRenderer;

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

	return success;
}

bool loadMedia(Tile* tiles[])
{
	//Loading success flag
	bool success = true;

	//Load test texture
	if (!gTest.loadFromFile("src/assets/hello_world.bmp"))
	{
		printf("Failed to load test image!\n");
		success = false;
	}
	else {
		//Clip sprite sheet

		//Set blend mode
		//gTest.setBlendMode(SDL_BLENDMODE_BLEND);
	}

	//Load dot texture
	if (!gDotTexture.loadFromFile("src/assets/dot.bmp"))
	{
		printf("Failed to load dot texture!\n");
		success = false;
	}

	//Load tile texture
	if (!gTileTexture.loadFromFile("src/assets/tiles.png"))
	{
		printf("Failed to load tile set texture!\n");
		success = false;
	}
	
	//Load tile map
	if (!setTiles(tiles))
	{
		printf("Failed to load tile set!\n");
		success = false;
	}

	//Open the font
	gFont = TTF_OpenFont("src/assets/lazy.ttf", 20);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else
	{
		//Set text color as black
		SDL_Color textColor = { 0, 0, 0, 255 };
	}

	return success;
}

void close(Tile* tiles[])
{
	//Deallocate tiles
	for (int i = 0; i < TOTAL_TILES; ++i)
	{
		if (tiles[i] != NULL)
		{
			delete tiles[i];
			tiles[i] = NULL;
		}
	}

	//Free loaded images
	gTest.free();
	gTileTexture.free();
	gDotTexture.free();
	gTimeTextTexture.free();

	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	//Destroy window and renderer
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool setTiles(Tile* tiles[])
{
	//Success flag
	bool tilesLoaded = true;

	//The tile offsets
	int x = 0, y = 0;

	//Open the map
	std::ifstream map("src/assets/lazy.map");

	//If the map couldn't be loaded
	if (map.fail())
	{
		printf("Unable to load map file!\n");
		tilesLoaded = false;
	}
	else
	{
		//Initialize the tiles
		for (int i = 0; i < TOTAL_TILES; ++i)
		{
			//Determines what kind of tile will be made
			int tileType = -1;

			//Read tile from map file
			map >> tileType;

			//If the was a problem in reading the map
			if (map.fail())
			{
				//Stop loading map
				printf("Error loading map: Unexpected end of file!\n");
				tilesLoaded = false;
				break;
			}

			//If the number is a valid tile number
			if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
			{
				tiles[i] = new Tile(x, y, tileType, &gTileTexture);
			}
			//If we don't recognize the tile type
			else
			{
				//Stop loading map
				printf("Error loading map: Invalid tile type at %d!\n", i);
				tilesLoaded = false;
				break;
			}

			//Move to next tile spot
			x += TILE_WIDTH;

			//If we've gone too far
			if (x >= LEVEL_WIDTH)
			{
				//Move back
				x = 0;

				//Move to the next row
				y += TILE_HEIGHT;
			}
		}

		//Clip the sprite sheet
		if (tilesLoaded)
		{
			//Red
			gTileTexture.setClipRegion(0, 0, TILE_WIDTH, TILE_HEIGHT);
			//Green
			gTileTexture.setClipRegion(0, 80, TILE_WIDTH, TILE_HEIGHT);
			//Blue
			gTileTexture.setClipRegion(0, 160, TILE_WIDTH, TILE_HEIGHT);
			//Center
			gTileTexture.setClipRegion(160, 80, TILE_WIDTH, TILE_HEIGHT);
			//Top
			gTileTexture.setClipRegion(160, 0, TILE_WIDTH, TILE_HEIGHT);
			//Top right
			gTileTexture.setClipRegion(240, 0, TILE_WIDTH, TILE_HEIGHT);
			//Right
			gTileTexture.setClipRegion(240, 80, TILE_WIDTH, TILE_HEIGHT);
			//Bottom right
			gTileTexture.setClipRegion(240, 160, TILE_WIDTH, TILE_HEIGHT);
			//Bottom
			gTileTexture.setClipRegion(160, 160, TILE_WIDTH, TILE_HEIGHT);
			//Bottom left
			gTileTexture.setClipRegion(80, 160, TILE_WIDTH, TILE_HEIGHT);
			//Left
			gTileTexture.setClipRegion(80, 80, TILE_WIDTH, TILE_HEIGHT);
			//Top left
			gTileTexture.setClipRegion(80, 0, TILE_WIDTH, TILE_HEIGHT);
		}
	}

	//Close the file
	map.close();

	//If the map was loaded fine
	return tilesLoaded;
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//The level tiles
		Tile* tileSet[TOTAL_TILES];

		//Load media
		if (!loadMedia(tileSet))
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop
			bool quit = false;

			//Event handler
			SDL_Event e;

			//Entity to move
			Entity entity(&gDotTexture);

			//Level camera
			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

			//Set text color as black
			SDL_Color textColor = { 0, 0, 0, 255 };

			//Current time start time
			Timer fpsTimer;

			//The frames per second cap timer
			Timer capTimer;

			//In memory text stream
			std::stringstream timeText;

			//Start counting frames per second
			int countedFrames = 0;
			fpsTimer.start();

			//While application is running
			while (!quit)
			{
				//Start cap timer
				capTimer.start();

				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quitting application
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}

					entity.handleEvent(e);

					/*
					//If user presses a key
					else if (e.type == SDL_KEYDOWN)
					{
						switch (e.key.keysym.sym)
						{
						case SDLK_UP:
							//Do something
							break;
						case SDLK_DOWN:
							//Do something
							break;
						case SDLK_LEFT:
							//Do something
							break;
						case SDLK_RIGHT:
							//Do something
							break;
						default:
							//Do something
							break;
						}
					}*/
					const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
					if (currentKeyStates[SDL_SCANCODE_UP])
					{
						//Do something
					}

					else if (currentKeyStates[SDL_SCANCODE_DOWN])
					{
						//Do something
					}

					else if (currentKeyStates[SDL_SCANCODE_LEFT])
					{
						//Do something
					}

					else if (currentKeyStates[SDL_SCANCODE_RIGHT])
					{
						//Do something
					}
					else {
						//Do something
					}
				}

				//Calculate and correct fps
				float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
				if (avgFPS > 2000000)
				{
					avgFPS = 0;
				}

				//Set text to be rendered
				timeText.str("");
				timeText << "FPS: " << avgFPS;

				//Render text
				if (!gTimeTextTexture.loadFromRenderedText(timeText.str().c_str(), textColor, gFont))
				{
					printf("Unable to render time texture!\n");
				}

				//Move the entity
				entity.move(tileSet,LEVEL_WIDTH, LEVEL_HEIGHT, TOTAL_TILES);
				entity.setCamera(camera, SCREEN_WIDTH, SCREEN_HEIGHT,LEVEL_WIDTH,LEVEL_HEIGHT);

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render level
				for (int i = 0; i < TOTAL_TILES; ++i)
				{
					tileSet[i]->render(camera);
				}

				//Render entities
				entity.render(camera);

				//Render test sprite to screen
				//gTest.setColor(255, 2, 53, 255 / 2);
				//gTest.render(0, 0, gTest.getCurrentClip());

				//Render fps counter
				gTimeTextTexture.render(5, 5);

				//Update screen
				SDL_RenderPresent(gRenderer);
				++countedFrames;

				//If frame finished early
				int frameTicks = capTimer.getTicks();
				if (frameTicks < SCREEN_TICKS_PER_FRAME)
				{
					//Wait remaining time
					SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
				}
			}
		}
		//Free resources and close SDL
		close(tileSet);
	}
	return 0;
}