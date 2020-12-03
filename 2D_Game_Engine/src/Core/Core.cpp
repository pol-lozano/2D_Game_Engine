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
#include "../Physics/Collision.h"

Core* Core::s_instance = nullptr;

Entity* player;
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
	auto wflags = (SDL_WINDOW_ALLOW_HIGHDPI);
	window = SDL_CreateWindow("ENGINE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, wflags);
	if (!window) std::cerr << SDL_GetError() << std::endl;

	//Create renderer
	renderer = SDL_CreateRenderer(window, -1, (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
	if (!renderer) std::cerr << SDL_GetError() << std::endl;

	AssetManager::get().loadTexture("test", "assets/player.png");
	AssetManager::get().loadTexture("test2", "assets/hello_world.bmp");
	AssetManager::get().loadFont("lazy", "assets/lazy.ttf", 20);

	//Create manager
	manager = new EntityManager();
	event = new SDL_Event();
	camera = new SDL_Rect{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	
	//Create entity
	player = new Entity(); 
	auto sprite = player->addComponent<Sprite>(renderer, "test");
	player->getComponent<Transform>().scale = Vector2(2,2);
	player->addComponent<Rigidbody2D>();
	player->addComponent<BoxCollider2D>(renderer, sprite.getWidth(), sprite.getHeight());
	player->addComponent<InputHandler>();
	player->addComponent<CharacterController2D>();
	player->addComponent<Text>(renderer, "Test", "lazy");

	entity1 = new Entity();
	entity1->getComponent<Transform>().position = Vector2(200, 100);
	auto sprite1 = entity1->addComponent<Sprite>(renderer, "test2");
	//entity1->addComponent<Rigidbody2D>();
	entity1->addComponent<BoxCollider2D>(renderer, sprite1.getWidth(), sprite1.getHeight(),"wall");
	
	entity2 = new Entity();
	auto sprite2 = entity2->addComponent<Sprite>(renderer, "test2");
	entity2->getComponent<Transform>().position = Vector2(0, 300);
	entity2->addComponent<BoxCollider2D>(renderer, sprite2.getWidth(), sprite2.getHeight(),"wall");

	manager->addEntity(player);
	manager->addEntity(entity1);
	manager->addEntity(entity2);

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
		default:
			break;
		}
	}
}


void Core::update(float dt)
{
	manager->refresh();
	manager->update(dt);	
	
	//Check for collisions
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
	camera->x = static_cast<int>(player->getComponent<Transform>().position.x) + player->getComponent<Sprite>().getWidth() - SCREEN_WIDTH / 2;
	camera->y = static_cast<int>(player->getComponent<Transform>().position.y) + player->getComponent<Sprite>().getHeight() - SCREEN_HEIGHT / 2;

	std::stringstream ss;
	ss << camera->x << "/" << camera->y;
	player->getComponent<Text>().setText(ss.str());
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
