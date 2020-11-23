#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>

struct ColliderComponent;

class Core
{
public:
	Core();
	~Core();

	void init(const char* title, int width, int height);

	void handleEvents();
	void update();
	void render();
	void close();
	bool running();

	//The window renderer
	static SDL_Renderer* gRenderer;
	static SDL_Event event;
	static bool isRunning;
	static SDL_Rect camera;

	enum groupLabels : std::size_t
	{
		groupMap,
		groupPlayers,
		groupEnemies,
		groupColliders
	};

private:
	//The window we'll be rendering to
	SDL_Window* gWindow;
	//Globally used font
	TTF_Font* gFont;
};

