#pragma once
#include <SDL.h>
#include <string>

#include "../Entity.h"
#include "../Component.h"
#include "../../AssetManager/AssetManager.h"
#include "../../WorldGen/Tilemap.h"
#include "../../Core/Core.h"

class TilemapRenderer : public Component {
public:
	TilemapRenderer(SDL_Renderer* target, std::string textureid, Tilemap* tilemap) : rTarget(target), textureID(textureid), map(tilemap) { }
	
	bool init() override final {
		transform = &entity->getComponent<Transform>();
		texture = AssetManager::get().getTexture(textureID);
		map->generate();

		return true;
	}

	void update(float dt) override final {
		dstRect.w = static_cast<int>(tileSize.x * transform->scale.x);
		dstRect.h = static_cast<int>(tileSize.y * transform->scale.y);

		dstRect.x = static_cast<int>(transform->position.x) - Core::get().getCamera()->x;
		dstRect.y = static_cast<int>(transform->position.y) - Core::get().getCamera()->y;
	}

	void draw() override final {
		for (int x = 0; x < map->getWidth(); x++) {
			for (int y = 0; y < map->getHeight(); y++) {
				int tileType = map->operator()(x,y);
				 
				SDL_Rect tilepos{ dstRect.x + tileSize.x * x, dstRect.y + tileSize.y * y , tileSize.x, tileSize.y } ;

				switch (tileType)
				{
				case 1:
					SDL_SetRenderDrawColor(rTarget, 255, 255, 255, 255);
					SDL_RenderFillRect(rTarget, &tilepos);
					break;
				case 0:
					SDL_SetRenderDrawColor(rTarget, 0, 0, 0, 255);
					SDL_RenderFillRect(rTarget, &tilepos);
					break;
				}
			}
		}
	}

private:
	Transform* transform = nullptr;
	SDL_Renderer* rTarget = nullptr;
	SDL_Texture* texture = nullptr;
	std::string textureID = "";

	SDL_Point tileSize = { 16, 16 };
	SDL_Rect srcRect = { 0, 0, 0, 0 };
	SDL_Rect dstRect = { 0, 0, 0, 0 };

	Tilemap* map = nullptr;

};
