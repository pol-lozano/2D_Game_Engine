#pragma once
#include <SDL.h>
#include <string>

#include "../Entity.h"
#include "../Component.h"

#include "../../AssetManager/AssetManager.h"
#include "../Physics/Collision.h" 

uint8_t TILE_SIZE = 16;

class TilemapRenderer : public Component {
public:
	TilemapRenderer(SDL_Renderer* target, std::string textureid) : rTarget(target), textureID(textureid) { }

	bool init() override final {
		transform = &entity->getComponent<Transform>();

		//Add tilemap if there isnt already one
		if (!entity->hasComponent<TilemapManager>()) { entity->addComponent<TilemapManager>(new Tilemap(512, 512)); }
		tilemapManager = &entity->getComponent<TilemapManager>();
		map = tilemapManager->getTilemap();

		//Load tilemap palette
		texture = AssetManager::get().getTexture(textureID);

		srcRect.w = 8;
		srcRect.h = 8;
		dstRect.w = TILE_SIZE;
		dstRect.h = TILE_SIZE;
		return true;
	}

	void update(float dt) override final {
		//Update camera pos and screen size

		//dstRect.x = static_cast<int>(transform->position.x) - Core::get().getCamera()->x;
		//dstRect.y = static_cast<int>(transform->position.y) - Core::get().getCamera()->y;
	}

	void draw() override final {
		const SDL_Rect visibleArea = Core::get().getVisibleArea();

		//Only draw all tiles that can be fully or partially seen.
		int start_x = floor(visibleArea.x / TILE_SIZE);
		int start_y = floor(visibleArea.y / TILE_SIZE);
		int end_x = floor(((visibleArea.x + visibleArea.w) / TILE_SIZE) + 2);
		int end_y = floor(((visibleArea.y + visibleArea.h) / TILE_SIZE) + 2);

		//Only render what is on screen
		for (int y = start_y; y <= end_y; y++) {
			for (int x = start_x; x <= end_x; x++) {
				//Check if in bounds
				if (!map->outOfBounds(x, y)) {
					int tile = map->getTileValue(x, y, 0);
					int subValue = map->getTileSubValue(x, y, 0);

					if (tile == 1) {
						switch (subValue) {
						case 0:
							srcRect.x = 8 * 14;
							srcRect.y = 8 * 6;
							break;
						case 1:
							srcRect.x = 8 * 14;
							srcRect.y = 8 * 1;
							break;
						case 2:
							srcRect.x = 8 * 9;
							srcRect.y = 8 * 6;
							break;
						case 3:
							//top left
							srcRect.x = 8 * 9;
							srcRect.y = 8 * 1;
							break;
						case 4:
							srcRect.x = 8 * 13;
							srcRect.y = 8 * 6;
							break;
						case 5:
							//top right
							srcRect.x = 8 * 13;
							srcRect.y = 8 * 1;
							break;
						case 6:
							srcRect.x = 8 * 10;
							srcRect.y = 8 * 6;
							break;
						case 7:
							//top
							srcRect.x = 8 * 11;
							srcRect.y = 8 * 1;
							break;
						case 8:
							srcRect.x = 8 * 14;
							srcRect.y = 8 * 5;
							break;
						case 9:
							srcRect.x = 8 * 14;
							srcRect.y = 8 * 2;
							break;
						case 10:
							//bottom left
							srcRect.x = 8 * 9;
							srcRect.y = 8 * 5;
							break;
						case 11:
							//left
							srcRect.x = 8 * 9;
							srcRect.y = 8 * 3;
							break;
						case 12:
							//bottom right
							srcRect.x = 8 * 13;
							srcRect.y = 8 * 5;
							break;
						case 13:
							//right
							srcRect.x = 8 * 13;
							srcRect.y = 8 * 3;
							break;
						case 14:
							//bottom
							srcRect.x = 8 * 11;
							srcRect.y = 8 * 5;
							break;
						case 15:
							//center
							srcRect.x = 8 * 0;
							srcRect.y = 8 * 9;
							break;
						default:
							srcRect.x = 8 * 19;
							srcRect.y = 8 * 8;
							break;
						}

						/*Shadows
						dstRect.x = (int)ceil(x * tileSize)+4 - Core::get().getCamera()->x;
						dstRect.y = (int)ceil(y * tileSize)+4 - Core::get().getCamera()->y;

						SDL_SetTextureColorMod(texture, 0, 0, 0);
						SDL_RenderCopy(rTarget, texture, &srcRect, &dstRect);*/

						//Tilemap
						dstRect.x = (int)floor(x * TILE_SIZE) - Core::get().getCamera()->x;
						dstRect.y = (int)floor(y * TILE_SIZE) - Core::get().getCamera()->y;

						SDL_SetTextureColorMod(texture, 255, 255, 255);
						SDL_RenderCopy(rTarget, texture, &srcRect, &dstRect);
					}
					else {
						if (x % 2 == 0 && y % 2 == 0) {
							srcRect.x = 8 * 2;
							srcRect.y = 8 * 9;
						}
						else {
							srcRect.x = 8 * 3;
							srcRect.y = 8 * 9;
						}

						dstRect.x = (int)floor(x * TILE_SIZE) - Core::get().getCamera()->x;
						dstRect.y = (int)floor(y * TILE_SIZE) - Core::get().getCamera()->y;
						SDL_SetTextureColorMod(texture, 200, 255, 200);
						SDL_RenderCopy(rTarget, texture, &srcRect, &dstRect);
					}
				}
			}
		}

		//Render mouse pos marker
		dstRect.x = (int)floor(tilemapManager->getMousePosTile().x * TILE_SIZE) - Core::get().getCamera()->x;
		dstRect.y = (int)floor(tilemapManager->getMousePosTile().y * TILE_SIZE) - Core::get().getCamera()->y;
		SDL_SetRenderDrawColor(rTarget, 86, 214, 255, 255);
		SDL_RenderDrawRect(rTarget, &dstRect);
	}

private:

	struct TilePalette {
		SDL_Point size;
		Uint8 tileSize = 8;

		std::map <Uint16, std::map<Uint8, Uint8>> tileTextureID{
			//Stone Bitmask to tileIndex
			{1,{0,158}},
			{1,{1,23}},
			{1,{2,89}},
			{1,{3,33}},
			{1,{4,157}},
			{1,{5,37}},
			{1,{6,154}},
			{1,{7,35}},
			{1,{8,134}},
			{1,{9,62}},
			{1,{10,129}},
			{1,{11,81}},
			{1,{12,133}},
			{1,{13,37}},
			{1,{14,131}},
			{1,{15,83}}
		};
		//Map spritesheet id to rect clip
		std::map<int, SDL_Rect> clips;

		void loadSpriteSheet(SDL_Texture* texture) {
			SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
			for (int x = 0; x < size.x / tileSize; x++) {
				for (int y = 0; y < size.y / tileSize; y++) {
					//Store all tileclips
					SDL_Rect temp { x * tileSize,y * tileSize,tileSize,tileSize };
					clips.emplace(getTileIndex(x, y), temp);
				}
			}
		}

		SDL_Rect getTileTexture(Uint8 value, Uint8 subValue) {
			//Return clip that has that value & subvalue
		}

		//Gets tile id by x and y pos
		inline int getTileIndex(int x, int y) {
			return (y * size.x) + x;
		}
	};

	Transform* transform = nullptr;
	TilemapManager* tilemapManager = nullptr;
	Tilemap* map = nullptr;

	SDL_Renderer* rTarget = nullptr;
	SDL_Texture* texture = nullptr;
	std::vector<SDL_Rect> tileTextures;

	std::string textureID = "";

	SDL_Rect srcRect = { 0, 0, 0, 0 };
	SDL_Rect dstRect = { 0, 0, 0, 0 };
	SDL_Rect tileRect = { 0, 0, 0, 0 }; //Tile rect will be inside of dst rect

	SDL_Point cameraPos = { 0, 0 };
	SDL_Point screenSize = { 0, 0 };
};
