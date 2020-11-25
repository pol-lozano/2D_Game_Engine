#include "Core.h"
#include <iostream>
#include "../AssetManager/AssetManager.h"
#include "../ECS/Components/Sprite.h"
#include "../ECS/Components/BoxCollider2D.h"
#include "../ECS/Components/Rigidbody2D.h"
#include "../ECS/Components/Transform.h"
#include "../Physics/Collision.h"

Core* Core::s_instance = nullptr;

Entity* entity;
Entity* entity1;

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
	auto wflags = (SDL_WINDOW_ALLOW_HIGHDPI);
	window = SDL_CreateWindow("ENGINE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, wflags);
	if (!window) std::cerr << SDL_GetError() << std::endl;

	//Create renderer
	renderer = SDL_CreateRenderer(window, -1, (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
	if (!renderer) std::cerr << SDL_GetError() << std::endl;

	AssetManager::get().loadTexture("test", "assets/doggo.png");
	AssetManager::get().loadTexture("test2", "assets/hello_world.bmp");

	//Create manager
	manager = new EntityManager();

	//Create entity
	entity = new Entity(); 
	auto sprite = entity->addComponent<Sprite>(renderer, "test");
	entity->getComponent<Transform>().scale = Vector2(2,2);
	entity->addComponent<Rigidbody2D>().setForceX(5);
	entity->addComponent<BoxCollider2D>(renderer, sprite.getWidth(), sprite.getHeight());
	
	entity1 = new Entity();
	entity1->getComponent<Transform>().position = Vector2(200, 100);
	auto sprite1 = entity1->addComponent<Sprite>(renderer, "test2");
	//entity1->addComponent<Rigidbody2D>();
	entity1->addComponent<BoxCollider2D>(renderer, sprite1.getWidth(), sprite1.getHeight());
	
	manager->addEntity(entity);
	manager->addEntity(entity1);

	clearColor = DARK;
	running = true;
}

void Core::events()
{
	//Handle events
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			quit();
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

	auto v = entity->getComponent<Transform>().position;
	std::cout << v << std::endl;;

	if (Collision::AABB(entity->getComponent<BoxCollider2D>(), entity1->getComponent<BoxCollider2D>())){
		entity->getComponent<Rigidbody2D>().setForceX(-10);
	}
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
