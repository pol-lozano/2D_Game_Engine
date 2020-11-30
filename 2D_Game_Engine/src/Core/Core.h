#pragma once
#include <SDL.h>
#include "../ECS/EntityManager.h"

struct ColliderComponent;
constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;
constexpr SDL_Color DARK = { 30,30,30,255 };

class Core
{
public:
	Core();
	~Core();

	void init();
	void quit();
	void clean();

	void events();
	void update(float dt);
	void render();

	//Singleton
	inline static Core& get() {
		if (s_instance == nullptr) {
			s_instance = new Core();
		}
		return *s_instance;
	}

	inline bool isRunning() {
		return running;
	}

	inline SDL_Rect* getCamera() {
		return camera;
	}

	inline SDL_Event* getEvent() {
		return event;
	}

	inline SDL_Renderer* getRenderer(){
		return renderer;
	}

private:
	EntityManager* manager;
	SDL_Event* event;

	bool running;
	SDL_Window* window;
	SDL_Color clearColor;
	SDL_Renderer* renderer;

	SDL_Rect* camera;

	static Core* s_instance;
};

