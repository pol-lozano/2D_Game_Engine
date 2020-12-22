#pragma once
#include <SDL.h>

#include "../Component.h"
#include "../Entity.h"
#include "../../WorldGen/Tilemap.h"
#include "../../Core/Core.h"

class TilemapManager : public Component {
public:
	TilemapManager( Tilemap* tilemap) : map(tilemap) { }

	void init() override final {
		transform = &entity->getComponent<Transform>();
	}

	void update(double dt) override final {
		//Update camera pos and screen size
		cameraPos = Core::get().getCameraPos();

		handleMouseEvents();
	}

	inline void handleMouseEvents() {
		//Get mouse position
		SDL_Point mousePos;
		SDL_GetMouseState(&mousePos.x, &mousePos.y);

		//Translate screen space to tile space
		mousePosTile = { (mousePos.x + cameraPos.x) / tileSize, (mousePos.y + cameraPos.y) / tileSize };

		//Check if in bounds
		if (!map->outOfBounds(mousePosTile.x, mousePosTile.y)) {
			switch (SDL_GetMouseState(NULL, NULL)) {
			case SDL_BUTTON(SDL_BUTTON_LEFT):
				//Only place if there isn't a tile on that position
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
	}

	inline Tilemap* getTilemap() {
		return map;
	}

	inline SDL_Point getMousePosTile() {
		return mousePosTile;
	}

private:
	Transform* transform = nullptr;
	Tilemap* map = nullptr;

	uint8_t tileSize = 16;

	SDL_Point cameraPos = { 0, 0 };
	SDL_Point mousePosTile = { 0, 0 };
};