#include "Core.h"
#include <iostream>
#include <sstream>
#include "../AssetManager/AssetManager.h"
#include "../ECS/Components/Sprite.h"
#include "../ECS/Components/BoxCollider2D.h"
#include "../ECS/Components/Rigidbody2D.h"
#include "../ECS/Components/Transform.h"
#include "../ECS/Components/CharacterController2D.h"
#include "../ECS/Components/InputHandler.h"
#include "../ECS/Components/Text.h"
#include "../ECS/Components/TilemapRenderer.h"
#include "../Physics/Collision.h"
#include "../WorldGen/Tilemap.h"

Core* Core::s_instance = nullptr;

Entity* player;
Entity* tilemap;

Entity* entity1;
Entity* entity2;

Core::Core()
{
	running = false;
	window = nullptr;
	renderer = nullptr;
}

Core::~Core(){}

void Core::init()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) std::cerr << SDL_GetError() << std::endl;
	
	//Create window
	auto wflags = (SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
	window = SDL_CreateWindow("ENGINE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, wflags);
	if (!window) std::cerr << SDL_GetError() << std::endl;

	//Create renderer
	renderer = SDL_CreateRenderer(window, -1, (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
	if (!renderer) std::cerr << SDL_GetError() << std::endl;

	//Get screen size
	display = new SDL_DisplayMode();
	SDL_GetDisplayMode(0, 0, display);

	SDL_SetWindowMinimumSize(window, 640, 480);

	//Set fullscreen
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

	AssetManager::get().loadTexture("tileset", "assets/tiles.png");
	AssetManager::get().loadTexture("test", "assets/player.png");
	AssetManager::get().loadFont("lazy", "assets/lazy.ttf", 20);

	//Create manager
	manager = new EntityManager();
	event = new SDL_Event();
	camera = new SDL_Rect{ 0, 0, display->w, display->h };

	//Create tilemap
	tilemap = new Entity();
	tilemap->addComponent<TilemapRenderer>(renderer,"tileset", new Tilemap(1024, 1024));

	//Create entity
	player = new Entity(); 
	auto sprite = player->addComponent<Sprite>(renderer, "test");
	player->getComponent<Transform>().scale = Vector2(1,1);
	player->addComponent<Rigidbody2D>();
	player->addComponent<BoxCollider2D>(renderer, sprite.getWidth(), sprite.getHeight());
	player->addComponent<InputHandler>();
	player->addComponent<CharacterController2D>();
	player->addComponent<Text>(renderer, 10, 10, "Test", "lazy");
	
	//TODO: Fix render queue
	manager->addEntity(tilemap);
	manager->addEntity(player);

	clearColor = DARK;
	running = true;
}

void Core::events()
{
	//Handle events
	while (SDL_PollEvent(event)) {
		switch (event->type) {
		case SDL_QUIT:
			quit();
			break;
		case SDL_WINDOWEVENT:
			switch (event->window.event) {
			case SDL_WINDOWEVENT_RESIZED:
				SDL_GetRendererOutputSize(renderer, &display->w, &display->h);
				camera->w = display->w;
				camera->h = display->h;
				std::cout << display->w << " " << display->h << std::endl;
				break;
			}
			break;
		default:
			break;
		}
	}
}


void Core::update(float dt)
{
	manager->refresh();
	manager->update(dt);	
	
	//Check for collisions todo fix better collision system
	for (auto c : colliders) {
		SDL_Rect overlap;

		if (Collision::AABB(player->getComponent<BoxCollider2D>(), *c, overlap))
		{
			if (c->getCollisionTag() == "wall") {
				//Resolve collision with intersection data, only correct shallow axis
				int dir;
				if (abs(overlap.h) <= abs(overlap.w)) {
					dir = (overlap.y + camera->y) - player->getComponent<Transform>().position.y - overlap.h / 2 <= 0 ? 1 : -1; //Get collision direction
					player->getComponent<Transform>().translateY(overlap.h * dir); //Correct player position
				}
				else {
					dir = (overlap.x + camera->x) - player->getComponent<Transform>().position.x - overlap.w / 2 <= 0 ? 1 : -1;
					player->getComponent<Transform>().translateX(overlap.w * dir);
				}
			}
		}
	}

	//Set camera position centered on player
	setCamera(player);

	std::stringstream ss;
	ss << static_cast<int>(player->getComponent<Transform>().position.x) << "/" << static_cast<int>(player->getComponent<Transform>().position.y);
	player->getComponent<Text>().setText(ss.str());
}

void Core::setCamera(Entity* target) {
	auto transform = target->getComponent<Transform>();
	auto sprite = target->getComponent<Sprite>();

 	camera->x = (static_cast<int>(transform.position.x) + sprite.getWidth() / 2) - display->w / 2;
	camera->y = (static_cast<int>(transform.position.y) + sprite.getHeight() / 2) - display->h / 2;
}

void Core::render()
{
	//Clear screen
	SDL_SetRenderDrawColor(renderer, clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	SDL_RenderClear(renderer);

	//Draw all entities to screen 
	manager->draw();

	//Update screen
	SDL_RenderPresent(renderer);
}

void Core::clean()
{
	//Clear textures and fonts
	AssetManager::get().clean();

	//Destroy window and renderer
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);	

	//Quit SDL subsystems
	SDL_Quit();
}

void Core::quit()
{
	running = false;
}
