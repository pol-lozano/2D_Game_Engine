#pragma once
#include <SDL.h>
#include <vector>
#include <algorithm>
#include <random>

#include "../../Component.h"
#include "../../../AssetManager/AssetManager.h"
#include "Chunk.h"

class Tilemap : public Component {
public:	
	Tilemap(SDL_Renderer* target, std::string textureid) : rTarget(target), textureID(textureid) { }

	void init() override final {
		//Load tilemap palette
		texture = AssetManager::get().getTexture(textureID);

		//Initialize block list
		for (Uint16 b = 0; b < 9; b++) {
			BLOCK_LIST.push_back(Vec2<Uint16>{ 0, 0 });
		}

		BLOCK_LIST[BlockID::DIRT].tile = { 0, 0 };
		BLOCK_LIST[BlockID::GRASS].tile = { 0, 1 };
		BLOCK_LIST[BlockID::STONE].tile = { 0, 2 };
		BLOCK_LIST[BlockID::WOOD].tile = { 0, 3 };
		BLOCK_LIST[BlockID::STONEBRICK].tile = { 0, 4 };
		BLOCK_LIST[BlockID::COPPER].tile = { 0, 5 };
		BLOCK_LIST[BlockID::IRON].tile = { 0, 6 };
		BLOCK_LIST[BlockID::SILVER].tile = { 0, 7 };

		//Initialize chunks
		ch_ptr = std::make_shared<Chunk>();

		for (Uint32 b = 0; b < CHUNK_AREA; b++) {
			ch_ptr->blocks.push_back({ BlockID::AIR, 0 });
			ch_ptr->lightUpdates.push_back(b);
		}

		ch_ptr->generate();
		ch_ptr->autoTile();
		ch_ptr->updateLight();

		Chunks.push_back(std::move(ch_ptr));
	}

	void update(double dt) override final {

	}

	void draw() override final {
		SDL_Rect tr {0, 0, TILE_SIZE, TILE_SIZE};

		const SDL_Rect visibleArea = Core::get().getVisibleArea();

		//Only draw all tiles that can be fully or partially seen.
		int start_x = floor(visibleArea.x / TILE_SIZE);
		int start_y = floor(visibleArea.y / TILE_SIZE);
		int end_x = floor(((visibleArea.x + visibleArea.w) / TILE_SIZE) + 2);
		int end_y = floor(((visibleArea.y + visibleArea.h) / TILE_SIZE) + 2);

		for (Uint32 c = 0; c < Chunks.size(); c++) {
			auto& ch_ptr = Chunks[c];

			ch_ptr->updateLight();

			//Only render what is on screen
			for (Sint16 ty = start_y; ty <= end_y; ty++) {
				for (Sint16 tx = start_x; tx <= end_x; tx++) {

					//BOUNDS CHECK
					if (tx < 0 || ty < 0 || tx > CHUNK_SIZE - 1 || ty > CHUNK_SIZE - 1) continue;
					auto& _bl = ch_ptr->blocks[ty * CHUNK_SIZE + tx];

					if (_bl.id == BlockID::AIR) continue;

					Uint8 _light = std::clamp((255 * _bl.light / 8) + 128, 0, 255);
					SDL_SetTextureColorMod(texture, _light, _light, _light);

					tr.x = floor((tx * TILE_SIZE) - visibleArea.x);
					tr.y = floor((ty * TILE_SIZE) - visibleArea.y);

					//Draw correct texture clip
					drawSpriteClip(texture, ((BLOCK_LIST[_bl.id].tile.x + _bl.sub) * TILE_SIZE), ((BLOCK_LIST[_bl.id].tile.y) * TILE_SIZE), &tr);

					
				}
			}

			//Get mouse position
			int MOUSE_X, MOUSE_Y;
			SDL_GetMouseState(&MOUSE_X, &MOUSE_Y);

			//Translate screen space to tile space
			Uint16 _tx = floor(Core::get().camToWorldX(MOUSE_X) / TILE_SIZE);
			Uint16 _ty = floor(Core::get().camToWorldY(MOUSE_Y) / TILE_SIZE);

			//Bounds check
			if (_tx > CHUNK_SIZE - 1 || _ty > CHUNK_SIZE - 1) continue;

			//Render marker
			tr.x = (int)floor((_tx * TILE_SIZE) - visibleArea.x);
			tr.y = (int)floor((_ty * TILE_SIZE) - visibleArea.y);

			if(Chunks[0]->blocks[_ty * CHUNK_SIZE + _tx].id == BlockID::AIR) { SDL_SetTextureColorMod(texture, 128, 255, 128); }
			else { SDL_SetTextureColorMod(texture, 255, 0, 128); }

			int currentTile = Chunks[0]->currentTile;
			SDL_SetTextureAlphaMod(texture, 150);
			drawSpriteClip(texture, ((BLOCK_LIST[currentTile].tile.x) * TILE_SIZE), ((BLOCK_LIST[currentTile].tile.y) * TILE_SIZE), &tr);
			SDL_SetTextureAlphaMod(texture, 255);

			tr.x = TILE_SIZE * 8;
			tr.y = TILE_SIZE * 2;

			//Display current tile
			SDL_SetTextureColorMod(texture, 255, 255, 255);
			drawSpriteClip(texture, ((BLOCK_LIST[currentTile].tile.x) * TILE_SIZE), ((BLOCK_LIST[currentTile].tile.y) * TILE_SIZE), &tr);
		} 
	}
	
	inline void drawSpriteClip(SDL_Texture* tex, Uint16 srcX, Uint16 srcY, SDL_Rect* dstRect) {
		SDL_Rect srcRect = { srcX, srcY, TILE_SIZE, TILE_SIZE };
		SDL_RenderCopy(rTarget, texture, &srcRect, dstRect);
	}

	//checks only first chunk gotta fix
	inline bool isSolidTile(Uint16 tx, Uint16 ty) {
		auto& ch_ptr = Chunks[0];

		//Bounds check
		if (tx < 0 || ty < 0 || tx > CHUNK_SIZE - 1 || ty > CHUNK_SIZE - 1) return true;
		auto& _bl = ch_ptr->blocks[ty * CHUNK_SIZE + tx];
		return _bl.id != BlockID::AIR;
	}

	inline int getCurrentTile() {
		auto& ch_ptr = Chunks[0];
		return ch_ptr->currentTile;
	}

	//Used for spawning the player in a proper location
	inline int getHighestPoint(Uint16 tx) {
		auto& ch_ptr = Chunks[0];
		if (tx < 0 || tx > CHUNK_SIZE - 1) return 0;
		for (Uint32 ty = 0; ty < CHUNK_SIZE; ty++){
			auto& _bl = ch_ptr->blocks[ty * CHUNK_SIZE + tx];
			if (_bl.id != 0) return ty - 2;
		}
		return CHUNK_SIZE;
	}

private:
	SDL_Renderer* rTarget = nullptr;
	SDL_Texture* texture = nullptr;
	std::string textureID;

	std::vector<BlockInfo> BLOCK_LIST;
	std::vector<std::shared_ptr<Chunk>> Chunks;
	std::shared_ptr<Chunk> ch_ptr;
};