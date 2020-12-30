#include "Core.h"
#include <iostream>
#include <sstream>

#include "../AssetManager/AssetManager.h"
#include "../Physics/Collision.h"

#include "../ECS/Components/Sprite.h"
#include "../ECS/Components/BoxCollider2D.h"
#include "../ECS/Components/Rigidbody2D.h"
#include "../ECS/Components/Transform.h"
#include "../ECS/Components/CharacterController2D.h"
#include "../ECS/Components/InputHandler.h"
#include "../ECS/Components/Text.h"
#include "../ECS/Components/TileMap.h"

Core* Core::s_instance = nullptr;

Entity* player;
Entity* tilemap;
Entity* entity1;
Entity* entity2;

Core::Core(){}
Core::~Core(){}

void Core::init()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) std::cerr << SDL_GetError() << std::endl;
	
	//Create window
	auto wflags = (SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
	window = SDL_CreateWindow("ENGINE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, wflags);
	if (!window) std::cerr << SDL_GetError() << std::endl;

	//Create renderer
	renderer = SDL_CreateRenderer(window, -1, (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
	if (!renderer) std::cerr << SDL_GetError() << std::endl;

	//Set blend mode
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	//Get screen size
	display = new SDL_DisplayMode();
	SDL_GetDisplayMode(0, 0, display);
	SDL_GetRendererOutputSize(renderer, &display->w, &display->h);
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
	tilemap->addComponent<Tilemap>(renderer, "tileset");

	//tilemap->addComponent<TilemapManager>(new Tilemap(1024, 1024));
	//tilemap->addComponent<TilemapRenderer>(renderer,"tileset");

	//Create entity
	player = new Entity(); 
	auto sprite = player->addComponent<Sprite>(renderer, "test");
	player->getComponent<Transform>().scale = Vec2F(2, 2);
	player->addComponent<Rigidbody2D>();
	player->addComponent<BoxCollider2D>(renderer, sprite.getWidth(), sprite.getHeight());
	player->addComponent<InputHandler>();
	player->addComponent<Text>(renderer, 10, 10, "Test", "lazy");
	player->addComponent<CharacterController2D>();

	entity1 = new Entity();
	entity1->getComponent<Transform>().position = Vec2F(200, 100);
	auto sprite1 = entity1->addComponent<Sprite>(renderer, "test");
	entity1->addComponent<BoxCollider2D>(renderer, sprite1.getWidth(), sprite1.getHeight(), "wall");
	entity1->getComponent<Transform>().scale = Vec2F(30,15);

	entity2 = new Entity();
	entity2->getComponent<Transform>().position = Vec2F(600, 150);
	auto sprite2 = entity2->addComponent<Sprite>(renderer, "test");
	entity2->addComponent<BoxCollider2D>(renderer, sprite2.getWidth(), sprite2.getHeight(), "wall");
	entity2->getComponent<Transform>().scale = Vec2F(30, 15);

	//TODO: Fix render queue
	manager->addEntity(tilemap);
	manager->addEntity(entity1);
	manager->addEntity(entity2);
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
				//Set camera size to display size
				camera->w = display->w;
				camera->h = display->h;
				printf("Changed screen size to : %d x %d\n", display->w, display->h);
				break;
			}
			break;
		default:
			break;
		}
	}
}

Vec2F oldPos; 

void Core::update(double dt)
{
	oldPos = player->getComponent<Transform>().position;;

	manager->refresh();
	manager->update(dt);	
	
	//Handle collisions
	handleCollisions();

	//Set camera position centered on player
	setCamera(player);
	
	std::stringstream ss;
	ss << static_cast<int>(player->getComponent<Transform>().position.x) << "/" << static_cast<int>(player->getComponent<Transform>().position.y);
	player->getComponent<Text>().setText(ss.str());
	
}

//Check for collisions todo fix better collision system
void Core::handleCollisions() {

	auto& pT = player->getComponent<Transform>();
	auto& pPos = pT.position;

	auto& pCol = player->getComponent<BoxCollider2D>();
	auto& box = pCol.getBox();

	auto& tMap = tilemap->getComponent<Tilemap>();

	Vec2F test;

	for (auto c : colliders) {
		SDL_Rect overlap;
		if (Collision::AABB(pCol, *c, overlap)) {
			if (c->getCollisionTag() == "wall") {
				if (pCol.resolveOverlap(overlap).length() > test.length())
					test = pCol.resolveOverlap(overlap);
			}
		}
	}

	//TILEMAP COLLISION
	Uint16 minX = pPos.x / TILE_SIZE;
	Uint16 minY = pPos.y / TILE_SIZE;
	Uint16 maxX = std::clamp((Uint32)(minX + (box.w / TILE_SIZE)), (Uint32)0, CHUNK_SIZE-1);
	Uint16 maxY = std::clamp((Uint32)(minY + (box.h / TILE_SIZE)), (Uint32)0, CHUNK_SIZE-1);

	Vec2F tCOld(65, 65);
	for (Uint16 tx = minX; tx <= maxX; tx++) {
		for (Uint16 ty = minY; ty <= maxY; ty++) {

			if (tMap.isSolidTile(tx, ty)) {
				SDL_Rect temp = { (tx)*TILE_SIZE - camera->x, (ty)*TILE_SIZE - camera->y, TILE_SIZE, TILE_SIZE };
				SDL_Rect overlap;

				if (Collision::AABB(&box, &temp, &overlap)) {
					if (pCol.resolveOverlap(overlap).length() > test.length())
						test = pCol.resolveOverlap(overlap);	
				}
			}
		}
	}

	//Correct deepest collision penetration
	pT.translate(test);
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
