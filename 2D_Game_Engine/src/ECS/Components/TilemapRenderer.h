#pragma once
#include <SDL.h>
#include <string>

#include "../Entity.h"
#include "../Component.h"
#include "../../AssetManager/AssetManager.h"
#include "../../WorldGen/Tilemap.h"
#include "../Physics/Collision.h"


class TilemapRenderer : public Component {
public:
	TilemapRenderer(SDL_Renderer* target, std::string textureid, Tilemap* tilemap) : rTarget(target), textureID(textureid), map(tilemap) { }
	
	bool init() override final {
		transform = &entity->getComponent<Transform>();
		texture = AssetManager::get().getTexture(textureID);

		srcRect.w = 8;
		srcRect.h = 8;

		return true;
	}

	void update(float dt) override final {
		//Update camera pos and screen size
		cameraPos = { Core::get().getCamera()->x, Core::get().getCamera()->y };
		screenSize = { Core::get().getDisplay()->w, Core::get().getDisplay()->h };

		dstRect.w = static_cast<int>(tileSize * transform->scale.x);
		dstRect.h = static_cast<int>(tileSize * transform->scale.y);
		
		SDL_Point mousePos;
		SDL_GetMouseState(&mousePos.x, &mousePos.y);
		mousePosTile.x = (mousePos.x + cameraPos.x) / tileSize;
		mousePosTile.y = (mousePos.y + cameraPos.y) / tileSize;	

		SDL_PumpEvents();

		//If in bounds
		if(!map->outOfBounds(mousePosTile.x, mousePosTile.y)){
			switch (SDL_GetMouseState(NULL, NULL)) {
			case SDL_BUTTON(SDL_BUTTON_LEFT):
				//Only place if there is not already a tile on that position
				if (!map->getTileValue(mousePosTile.x, mousePosTile.y, 0)) {
					map->setTile(mousePosTile.x, mousePosTile.y, 0, 1, 1);
					map->autoTileNeighbours(mousePosTile.x, mousePosTile.y, 0);
				}
				break;
			case SDL_BUTTON(SDL_BUTTON_RIGHT):
				//Get world pos
				if (map->getTileValue(mousePosTile.x, mousePosTile.y, 0)) {
					map->setTile(mousePosTile.x, mousePosTile.y, 0, 0, 1);
					map->autoTileNeighbours(mousePosTile.x, mousePosTile.y, 0);
				}
				break;
			}
		}

		//dstRect.x = static_cast<int>(transform->position.x) - Core::get().getCamera()->x;
		//dstRect.y = static_cast<int>(transform->position.y) - Core::get().getCamera()->y;
	}
	
	void draw() override final {
		//Only render what is on screen
		for (int y = floor(cameraPos.y / tileSize) - tileSize; y <= floor((cameraPos.y + screenSize.y + tileSize) / tileSize); y++) {
			for (int x = floor(cameraPos.x / tileSize) - tileSize; x <= floor((cameraPos.x + screenSize.x + tileSize) / tileSize); x++) {

				//Check if in bounds
				if (!map->outOfBounds(x,y)) {
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

						dstRect.x = (int)ceil(x * tileSize) - Core::get().getCamera()->x;
						dstRect.y = (int)ceil(y * tileSize) - Core::get().getCamera()->y;

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

						dstRect.x = (int)floor(x * tileSize) - Core::get().getCamera()->x;
						dstRect.y = (int)floor(y * tileSize) - Core::get().getCamera()->y;
						SDL_SetTextureColorMod(texture, 200, 255, 200);
						SDL_RenderCopy(rTarget, texture, &srcRect, &dstRect);
					}
				}
			}
		}
		//Render mouse pos marker
		dstRect.x = (int)floor(mousePosTile.x * tileSize) - Core::get().getCamera()->x;
		dstRect.y = (int)floor(mousePosTile.y * tileSize) - Core::get().getCamera()->y;
		SDL_SetRenderDrawColor(rTarget, 86, 214, 255, 255);
		SDL_RenderDrawRect(rTarget, &dstRect);
		/*
		for (int x = 0; x < map->getWidth(); x++) {
			for (int y = 0; y < map->getHeight(); y++) {
				//srcRect.x = tile % palettetilewidth * tileSize;
				//srcRect.y = tile / palettetilewidth * tileSize;
				int tile = map->getTileValue(x, y, 0);
				int subValue = map->getTileSubValue(x, y, 0);

				if(tile == 1){
					switch (subValue) {
						//CORNERS
					case 3:
						//top left
						srcRect.x = 8 * 9;
						srcRect.y = 8 * 1;
						break;
					case 5:
						//top right
						srcRect.x = 8 * 13;
						srcRect.y = 8 * 1;
						break;
					case 10:
						//bottom left
						srcRect.x = 8 * 9;
						srcRect.y = 8 * 5;
						break;
					case 12:
						//bottom right
						srcRect.x = 8 * 13;
						srcRect.y = 8 * 5;
						break;
						//SIDES
					case 7:
						//top
						srcRect.x = 8 * 11;
						srcRect.y = 8 * 1;
						break;
					case 11:
						//left
						srcRect.x = 8 * 9;
						srcRect.y = 8 * 3;
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

					srcRect.w = 8;
					srcRect.h = 8;

					dstRect.x = (int)floor(x * tileSize) - Core::get().getCamera()->x;
					dstRect.y = (int)floor(y * tileSize) - Core::get().getCamera()->y;

					SDL_SetTextureColorMod(texture, 255, 255, 255);
					SDL_RenderCopy(rTarget, texture, &srcRect, &dstRect);
				}/*
				else {
					if (x % 2 == 0 && y %2 ==0 ) {
						srcRect.x = 8 * 2;
						srcRect.y = 8 * 9;
					}
					else {
						srcRect.x = 8 * 3;
						srcRect.y = 8 * 9;
					}
					srcRect.w = 8;
					srcRect.h = 8;

					dstRect.x = (int)floor(x * tileSize) - Core::get().getCamera()->x;
					dstRect.y = (int)floor(y * tileSize) - Core::get().getCamera()->y;
					SDL_SetTextureColorMod(texture, 200, 200, 200);
					SDL_RenderCopy(rTarget, texture, &srcRect, &dstRect);
				}
			}
		}*/
	}

private:
	Transform* transform = nullptr;
	SDL_Renderer* rTarget = nullptr;
	SDL_Texture* texture = nullptr;
	std::string textureID = "";

	uint8_t tileSize = 16;
	SDL_Rect srcRect = { 0, 0, 0, 0 };
	SDL_Rect dstRect = { 0, 0, 0, 0 };
	SDL_Rect tileRect = { 0, 0, 0, 0 }; //Tile rect will be inside of dst rect

	Tilemap* map = nullptr;

	SDL_Point mousePosTile = {0, 0};
	SDL_Point cameraPos = { 0, 0 };
	SDL_Point screenSize = { 0, 0 };
};
