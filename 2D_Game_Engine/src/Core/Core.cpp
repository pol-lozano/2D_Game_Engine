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
Entity* background;
Entity* uiLabel;

Entity* pickup1;
Entity* pickup2;
Entity* pickup3;
Entity* pickup4;
Entity* pickup5;

Core::Core(){}
Core::~Core(){}

void Core::init() {
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) std::cerr << SDL_GetError() << std::endl;
	
	//Create window
	auto wflags = (SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
	window = SDL_CreateWindow("ENGINE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, wflags);
	if (!window) std::cerr << SDL_GetError() << std::endl;

	//Create renderer
	renderer = SDL_CreateRenderer(window, -1, (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
	if (!renderer) std::cerr << SDL_GetError() << std::endl;

	//For gradients
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

	//Set blend mode
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	//Get screen size
	display = new SDL_DisplayMode();
	SDL_GetDisplayMode(0, 0, display);
	SDL_GetRendererOutputSize(renderer, &display->w, &display->h);
	SDL_SetWindowMinimumSize(window, 640, 480);

	//Set fullscreen
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

	//Load media
	AssetManager::get().loadTexture("tileset", "assets/tiles.png");
	AssetManager::get().loadTexture("sky", "assets/sky.png");
	AssetManager::get().loadTexture("test", "assets/player.png");
	AssetManager::get().loadFont("andy", "assets/andy.ttf", 16);

	//Create manager
	manager = new EntityManager();
	event = new SDL_Event();
	camera = new SDL_Rect{ 0, 0, display->w, display->h };

	//Create tilemap
	tilemap = new Entity();
	auto map = tilemap->addComponent<Tilemap>(renderer, "tileset");
	tilemap->addComponent<Text>(renderer, 16, 32, "Test", "andy");

	//Player
	player = new Entity(); 
	auto sprite = player->addComponent<Sprite>(renderer, "test");
	player->getComponent<Transform>().scale = Vec2F(2, 2);

	//Spawn player in the middle of the map
	player->getComponent<Transform>().position = Vec2F((CHUNK_SIZE / 2) * TILE_SIZE, map.getHighestPoint(CHUNK_SIZE / 2) * TILE_SIZE);
	player->addComponent<Rigidbody2D>();
	player->addComponent<BoxCollider2D>(renderer, sprite.getWidth(), sprite.getHeight());
	player->addComponent<InputHandler>();
	player->addComponent<Text>(renderer, 16, 16, "Test", "andy");
	player->addComponent<CharacterController2D>();

	//Background gradient
	background = new Entity();
	auto bgSprite = background->addComponent<Sprite>(renderer, "sky");
	background->getComponent<Transform>().scale = Vec2F(CHUNK_SIZE * TILE_SIZE, CHUNK_SIZE * TILE_SIZE/ bgSprite.getHeight());
	
	//UI
	uiLabel = new Entity();
	uiLabel->addComponent<Text>(renderer, 16, 48, "Test", "andy");

	//Pickups
	pickup1 = new Entity();
	pickup1->getComponent<Transform>().position = Vec2F((CHUNK_SIZE / 3) * TILE_SIZE, map.getHighestPoint(CHUNK_SIZE / 3) * TILE_SIZE);
	auto pickup1sprite = pickup1->addComponent<Sprite>(renderer, "test");
	pickup1->addComponent<BoxCollider2D>(renderer, pickup1sprite.getWidth(), pickup1sprite.getHeight(), "pickup");
	pickup1->getComponent<Transform>().scale = Vec2F(2, 2);

	pickup2 = new Entity();
	pickup2->getComponent<Transform>().position = Vec2F((CHUNK_SIZE -(CHUNK_SIZE / 3)) * TILE_SIZE, map.getHighestPoint(CHUNK_SIZE- (CHUNK_SIZE / 3)) * TILE_SIZE);
	auto pickup2sprite = pickup2->addComponent<Sprite>(renderer, "test");
	pickup2->addComponent<BoxCollider2D>(renderer, pickup2sprite.getWidth(), pickup2sprite.getHeight(), "pickup");
	pickup2->getComponent<Transform>().scale = Vec2F(2, 2);

	pickup3 = new Entity();
	pickup3->getComponent<Transform>().position = Vec2F((CHUNK_SIZE / 5) * TILE_SIZE, map.getHighestPoint(CHUNK_SIZE / 5) * TILE_SIZE);
	auto pickup3sprite = pickup3->addComponent<Sprite>(renderer, "test");
	pickup3->addComponent<BoxCollider2D>(renderer, pickup3sprite.getWidth(), pickup3sprite.getHeight(), "pickup");
	pickup3->getComponent<Transform>().scale = Vec2F(2, 2);

	pickup4 = new Entity();
	pickup4->getComponent<Transform>().position = Vec2F((CHUNK_SIZE - (CHUNK_SIZE / 6)) * TILE_SIZE, map.getHighestPoint(CHUNK_SIZE - (CHUNK_SIZE / 6)) * TILE_SIZE);
	auto pickup4sprite = pickup4->addComponent<Sprite>(renderer, "test");
	pickup4->addComponent<BoxCollider2D>(renderer, pickup4sprite.getWidth(), pickup4sprite.getHeight(), "pickup");
	pickup4->getComponent<Transform>().scale = Vec2F(2, 2);

	pickup5 = new Entity();
	pickup5->getComponent<Transform>().position = Vec2F((CHUNK_SIZE / 7) * TILE_SIZE, map.getHighestPoint(CHUNK_SIZE / 7) * TILE_SIZE);
	auto pickup5sprite = pickup5->addComponent<Sprite>(renderer, "test");
	pickup5->addComponent<BoxCollider2D>(renderer, pickup5sprite.getWidth(), pickup5sprite.getHeight(), "pickup");
	pickup5->getComponent<Transform>().scale = Vec2F(2, 2);

	//TODO: Fix render queue
	manager->addEntity(background);
	manager->addEntity(tilemap);
	manager->addEntity(pickup1);
	manager->addEntity(pickup2);
	manager->addEntity(pickup3);
	manager->addEntity(pickup4);
	manager->addEntity(pickup5);
	manager->addEntity(player);
	manager->addEntity(uiLabel);

	clearColor = DARK;
	running = true;
}

void Core::events() {
	//Handle events
	while (SDL_PollEvent(event)) {
		switch (event->type) {
		case SDL_QUIT:
			quit();
			break;
		case SDL_WINDOWEVENT:
			switch (event->window.event) {
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				//Set camera size to display size
				SDL_GetRendererOutputSize(renderer, &display->w, &display->h);
				camera->w = display->w;
				camera->h = display->h;
				printf("Changed screen size to : %d x %d\n", display->w, display->h);
				break;
			}
			break;
		case SDL_KEYDOWN:
			switch (event->key.keysym.sym) {
			case SDLK_ESCAPE:
				//Exit full screen
				SDL_SetWindowFullscreen(window, 0);
				break;
			}
			break;
		default:
			break;
		}
	}
}

void Core::update(double dt) {
	manager->refresh();
	manager->update(dt);	
	updateUI(dt);

	//Handle collisions
	handleCollisions();

	//Set camera position centered on player
	setCamera(player);
}

//Check for collisions TODO: fix better collision system
void Core::handleCollisions() {

	auto& pT = player->getComponent<Transform>();
	auto& pPos = pT.position;

	auto& pCol = player->getComponent<BoxCollider2D>();
	auto& box = pCol.getBox();

	auto& tMap = tilemap->getComponent<Tilemap>();

	Vec2F test;

	//BOXCOLLIDER2D COLLISION
	for (auto c : colliders) {
		SDL_Rect overlap;
		//PLAYER
		if (Collision::AABB(pCol, *c, overlap)) {
			if (c->getCollisionTag() == "wall") {
				if (pCol.resolveOverlap(overlap).length() > test.length())
					test = pCol.resolveOverlap(overlap);
			}
			if (c->getCollisionTag() == "pickup") {
				if (pCol.resolveOverlap(overlap).length() > test.length())
					test = pCol.resolveOverlap(overlap);
				//Add pickup

				//Destroy entity
				c->entity->destroy();
				//remove this collider from list
				colliders.erase(std::remove(colliders.begin(), colliders.end(), c), colliders.end());
			}
		}
	}

	//TILEMAP COLLISION
	Uint16 minX = pPos.x / TILE_SIZE;
	Uint16 minY = pPos.y / TILE_SIZE;
	Uint16 maxX = std::clamp((Uint32)(minX + (box.w / TILE_SIZE)), (Uint32)0, CHUNK_SIZE-1);
	Uint16 maxY = std::clamp((Uint32)(minY + (box.h / TILE_SIZE)), (Uint32)0, CHUNK_SIZE-1);

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

	//Correct only deepest collision penetration (BUGGED)
	pT.translate(test);
}

void Core::updateUI(double dt) {
	std::stringstream ss;
	//ss << player->getComponent<Transform>().position;
	ss << "Fps: " << 1 / dt;
	player->getComponent<Text>().setText(ss.str());

	std::stringstream ss2;
	int tileID = tilemap->getComponent<Tilemap>().getCurrentTile();
	ss2 << "Chunk Size: " << (int)CHUNK_SIZE << " Tile Size: " << (int)TILE_SIZE;
	uiLabel->getComponent<Text>().setText(ss2.str());

	std::stringstream ss3;
	ss3 << "Current Tile ID: " << tileID;
	tilemap->getComponent<Text>().setText(ss3.str());
}

void Core::setCamera(Entity* target) {
	Vec2F pPos = target->getComponent<Transform>().position;
	auto sprite = target->getComponent<Sprite>();
	
 	camera->x = (static_cast<int>(pPos.x) + sprite.getWidth() / 2) - display->w / 2;
	camera->y = (static_cast<int>(pPos.y) + sprite.getHeight() / 2) - display->h / 2;
	
	//Camera limits
	if (camera->x < 0) { camera->x = 0; }
	if (camera->y < 0) { camera->y = 0; }
	if (camera->x > (CHUNK_SIZE * TILE_SIZE) - camera->w) { camera->x = (CHUNK_SIZE * TILE_SIZE) - camera->w; }
	if (camera->y > (CHUNK_SIZE * TILE_SIZE) - camera->h) { camera->y = (CHUNK_SIZE * TILE_SIZE) - camera->h; }
}

void Core::render() {
	//Clear screen
	SDL_SetRenderDrawColor(renderer, clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	SDL_RenderClear(renderer);

	//Draw all entities to screen 
	manager->draw();

	//Update screen
	SDL_RenderPresent(renderer);
}

void Core::clean() {
	//Clear textures and fonts
	AssetManager::get().clean();

	//Destroy window and renderer
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);	

	//Quit SDL subsystems
	SDL_Quit();
}
