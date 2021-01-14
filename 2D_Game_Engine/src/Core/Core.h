#pragma once
#include <SDL.h>
#include <vector>
#include "../ECS/EntityManager.h"

class BoxCollider2D;

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 960;
constexpr SDL_Color DARK = { 16, 16, 16, 255 };

class Core
{
public:
	Core() = default;
	~Core() = default;

	//Singleton
	inline static Core& get() {
		if (s_instance == nullptr) {
			s_instance = new Core();
		}
		return *s_instance;
	}

	void init();
	inline void quit() { running = false; }
	void clean();

	void events();
	void update(double dt);
	void render();

	inline bool isRunning() { return running; }
	inline SDL_Renderer* getRenderer() { return renderer; }
	inline SDL_Event* getEvent() { return event; }

	//COLLISION
	inline std::vector<BoxCollider2D*> getColliders() { return colliders; }
	inline void addCollider(BoxCollider2D* col) { colliders.push_back(col); }
	void handleCollisions(double dt);

	//CAMERA 
	void setCamera(Entity* target);
	void updateUI(double dt);

	//Returns the area that the camera can see, For camera culling
	inline SDL_Rect getVisibleArea() { return SDL_Rect{ camera->x, camera->y, display->w, display->h }; }
	inline int camToWorldX(int x) { return x + camera->x; }
	inline int camToWorldY(int y) { return y + camera->y; }

private:
	static Core* s_instance;
	bool running = false;

	EntityManager* manager = nullptr;
	SDL_Event* event = nullptr;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_DisplayMode* display = nullptr;
	SDL_Rect* camera = nullptr;

	SDL_Color clearColor = DARK;

	std::vector<BoxCollider2D*> colliders;
};

