#pragma once
#include <SDL.h>
#include "../ECS/EntityManager.h"
#include <vector>

class BoxCollider2D;

constexpr int SCREEN_WIDTH = 640*2;
constexpr int SCREEN_HEIGHT = 480*2;
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
	void setCamera(Entity* target);
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

	inline SDL_Point getCameraPos() {
		return SDL_Point{ camera->x, camera->y };
	}

	inline int camToWorldX(int x) {
		return x + camera->x;
	}

	inline int camToWorldY(int y) {
		return y + camera->y;
	}

	inline SDL_Point getScreenSize() {
		return SDL_Point {display->w, display->h};
	}

	//Returns the area that the camera can see, For camera culling
	inline SDL_Rect getVisibleArea() {
		return SDL_Rect { camera->x, camera->y, display->w, display->h };
	}

	inline SDL_Event* getEvent() {
		return event;
	}

	inline std::vector<BoxCollider2D*> getColliders() {
		return colliders;
	}

	inline void addCollider(BoxCollider2D* col) {
		colliders.push_back(col);
	}

	inline SDL_Renderer* getRenderer(){
		return renderer;
	}

private:
	EntityManager* manager;
	SDL_Event* event;

	bool running;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_DisplayMode* display;

	SDL_Color clearColor;
	SDL_Rect* camera;

	std::vector<BoxCollider2D*> colliders;

	static Core* s_instance;
};

