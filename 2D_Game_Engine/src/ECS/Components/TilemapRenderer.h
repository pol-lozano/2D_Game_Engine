#pragma once
#include <SDL.h>
#include <string>

#include "../Entity.h"
#include "../Component.h"

#include "../../AssetManager/AssetManager.h"
#include "../Physics/Collision.h" 


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

		return true;
	}

	void update(float dt) override final {
		//Update camera pos and screen size
		cameraPos = Core::get().getCameraPos();
		screenSize = Core::get().getScreenSize();

		dstRect.w = static_cast<int>(tileSize * transform->scale.x);
		dstRect.h = static_cast<int>(tileSize * transform->scale.y);

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

						/*Shadows
						dstRect.x = (int)ceil(x * tileSize)+4 - Core::get().getCamera()->x;
						dstRect.y = (int)ceil(y * tileSize)+4 - Core::get().getCamera()->y;

						SDL_SetTextureColorMod(texture, 0, 0, 0);
						SDL_RenderCopy(rTarget, texture, &srcRect, &dstRect);*/

						//Tilemap
						dstRect.x = (int)floor(x * tileSize) - Core::get().getCamera()->x;
						dstRect.y = (int)floor(y * tileSize) - Core::get().getCamera()->y;

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
		dstRect.x = (int)floor(tilemapManager->getMousePosTile().x * tileSize) - Core::get().getCamera()->x;
		dstRect.y = (int)floor(tilemapManager->getMousePosTile().y * tileSize) - Core::get().getCamera()->y;
		SDL_SetRenderDrawColor(rTarget, 86, 214, 255, 255);
		SDL_RenderDrawRect(rTarget, &dstRect);
	}

private:
	Transform* transform = nullptr;
	TilemapManager* tilemapManager = nullptr;
	Tilemap* map = nullptr;

	SDL_Renderer* rTarget = nullptr;
	SDL_Texture* texture = nullptr;
	std::string textureID = "";

	uint8_t tileSize = 16;

	SDL_Rect srcRect = { 0, 0, 0, 0 };
	SDL_Rect dstRect = { 0, 0, 0, 0 };
	SDL_Rect tileRect = { 0, 0, 0, 0 }; //Tile rect will be inside of dst rect

	SDL_Point cameraPos = { 0, 0 };
	SDL_Point screenSize = { 0, 0 };
};
