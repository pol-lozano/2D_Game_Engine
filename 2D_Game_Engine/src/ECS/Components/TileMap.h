#pragma once
#include <SDL.h>

#include "../Component.h"
#include "../Entity.h"
#include <vector>
#include <algorithm>
#include <memory>
#include "../../Core/Core.h"

enum BlockID : Uint16 {
	AIR,
	GRASS,
	DIRT,
	STONE,
	WOOD
};

struct Block {
	BlockID id = BlockID::AIR; //Block type
	Uint8 hp = 1; //Block health
	Uint8 sub = 0; //Block subvalue
	Uint8 light = 0; //Light value
};

//Store tile texture info
struct BlockInfo {
	BlockInfo(Vec2<Uint16> t) : tile(t) {}
	Vec2<Uint16> tile;
};

const Uint8 TILE_SIZE = 16;
const Uint32 CHUNK_SIZE = 256;
const Uint32 CHUNK_AREA = CHUNK_SIZE * CHUNK_SIZE;

struct Chunk {
	std::vector<Block> blocks;
	std::vector<Uint32> lightUpdates;

	void updateLight() {
		//Get mouse position
		int MOUSE_X, MOUSE_Y;
		Uint32 MOUSE_STATE = SDL_GetMouseState(&MOUSE_X, &MOUSE_Y);

		//Translate screen space to tile space
		Uint16 _tx = floor(Core::get().camToWorldX(MOUSE_X) / TILE_SIZE);
		Uint16 _ty = floor(Core::get().camToWorldY(MOUSE_Y) / TILE_SIZE);

		//Bounds check
		if (_tx < CHUNK_SIZE && _ty < CHUNK_SIZE) {
			//Handle Mouse input
			if (MOUSE_STATE == SDL_BUTTON(SDL_BUTTON_LEFT) || MOUSE_STATE == SDL_BUTTON(SDL_BUTTON_RIGHT)) {

				//Place or remove blocks
				switch (MOUSE_STATE) {
				case SDL_BUTTON(SDL_BUTTON_LEFT):
					if(blocks[_ty * CHUNK_SIZE + _tx].id == 0)
					blocks[std::clamp(_ty * CHUNK_SIZE + _tx, (Uint32)0, CHUNK_AREA - 1)].id = BlockID::STONE;
					break;
				case SDL_BUTTON(SDL_BUTTON_RIGHT):
					blocks[std::clamp(_ty * CHUNK_SIZE + _tx, (Uint32)0, CHUNK_AREA - 1)].id = BlockID::AIR;
					break;
				}

				//Push to light update queue
				//Update self
				lightUpdates.push_back(std::clamp(_ty * CHUNK_SIZE + _tx, (Uint32)0, CHUNK_AREA - 1));

				lightUpdates.push_back(std::clamp(_ty * CHUNK_SIZE + (_tx + 1), (Uint32)0, CHUNK_AREA - 1));
				lightUpdates.push_back(std::clamp(_ty * CHUNK_SIZE + (_tx - 1), (Uint32)0, CHUNK_AREA - 1));
				lightUpdates.push_back(std::clamp((_ty + 1) * CHUNK_SIZE + (_tx), (Uint32)0, CHUNK_AREA - 1));
				lightUpdates.push_back(std::clamp((_ty - 1) * CHUNK_SIZE + (_tx), (Uint32)0, CHUNK_AREA - 1));

				lightUpdates.push_back(std::clamp((_ty - 1) * CHUNK_SIZE + (_tx - 1), (Uint32)0, CHUNK_AREA - 1));
				lightUpdates.push_back(std::clamp((_ty - 1) * CHUNK_SIZE + (_tx + 1), (Uint32)0, CHUNK_AREA - 1));
				lightUpdates.push_back(std::clamp((_ty + 1) * CHUNK_SIZE + (_tx - 1), (Uint32)0, CHUNK_AREA - 1));
				lightUpdates.push_back(std::clamp((_ty + 1) * CHUNK_SIZE + (_tx + 1), (Uint32)0, CHUNK_AREA - 1));
			}

			if (!lightUpdates.empty()) {

				Uint32 lu_s = lightUpdates.size();
				BlockID id = BlockID::AIR;
				Uint32 tile;
				Uint8 sub;

				Uint32 tl, tr, tt, tb;
				Uint32 ttl, ttr, tbl, tbr;

				//Update tiles in update queue
				for (Uint32 l = 0; l < lu_s; l++) {
					//Light update

					tile = lightUpdates.back();
					lightUpdates.pop_back();

					tl = (blocks[std::clamp((Sint32)tile - 1, 0, (Sint32)CHUNK_AREA - 1)].id != BlockID::AIR);
					tr = (blocks[std::clamp((Sint32)tile + 1, 0, (Sint32)CHUNK_AREA - 1)].id != BlockID::AIR);
					tt = (blocks[std::clamp((Sint32)tile - (Sint32)CHUNK_SIZE, 0, (Sint32)CHUNK_AREA - 1)].id != BlockID::AIR);
					tb = (blocks[std::clamp((Sint32)tile + (Sint32)CHUNK_SIZE, 0, (Sint32)CHUNK_AREA - 1)].id != BlockID::AIR);

					//Corners
					ttl = (blocks[std::clamp(((Sint32)tile - (Sint32)CHUNK_SIZE) - 1, 0, (Sint32)CHUNK_AREA - 1)].id != BlockID::AIR);
					ttr = (blocks[std::clamp(((Sint32)tile - (Sint32)CHUNK_SIZE) + 1, 0, (Sint32)CHUNK_AREA - 1)].id != BlockID::AIR);
					tbl = (blocks[std::clamp(((Sint32)tile + (Sint32)CHUNK_SIZE) - 1, 0, (Sint32)CHUNK_AREA - 1)].id != BlockID::AIR);
					tbr = (blocks[std::clamp(((Sint32)tile + (Sint32)CHUNK_SIZE) + 1, 0, (Sint32)CHUNK_AREA - 1)].id != BlockID::AIR);

					//TODO : fix proper lights
					blocks[tile].light = (Uint8)(!tl || !tr || !tt || !tb || !ttl || !ttr || !tbl || !tbr);

					//Autotile 
					id = blocks[tile].id;

					if (id == BlockID::AIR) continue;

					sub = (Uint8)(tt +
						(tr << 1) +
						(tb << 2) +
						(tl << 3));
					/*
						if (sub == 15 && (ttl || ttr || tbl || tbr)) {
							sub += (Uint8)(ttl +
								(ttr << 1) +
								(tbr << 2) +
								(tbl << 3));
						}
					*/

					blocks[tile].sub = sub;
				}

			}
		}
	}

	//Autotile chunk
	void autoTile() {
		BlockID id = BlockID::AIR;
		Uint8 sub;

		for (Uint16 y = 1; y < CHUNK_SIZE - 1; y++) {
			for (Uint16 x = 1; x < CHUNK_SIZE - 1; x++) {
				//Get tile id
				id = blocks[y * CHUNK_SIZE + x].id;

				if (id <= 0) continue;

				//Calculate subvalue with bitshifting
				sub = ((Uint8)(blocks[(y - 1) * CHUNK_SIZE + (x)].id != BlockID::AIR)) +
					  ((Uint8)(blocks[(y) * CHUNK_SIZE + (x + 1)].id != BlockID::AIR) << 1) +
					  ((Uint8)(blocks[(y + 1) * CHUNK_SIZE + (x)].id != BlockID::AIR) << 2) +
					  ((Uint8)(blocks[(y) * CHUNK_SIZE + (x - 1)].id != BlockID::AIR) << 3);

				blocks[y * CHUNK_SIZE + x].sub = sub;
			}
		}
	}
};

class Tilemap : public Component {
public:	
	Tilemap(SDL_Renderer* target, std::string textureid) : rTarget(target), textureID(textureid) { }

	void init() override final {
		//Load tilemap palette
		texture = AssetManager::get().getTexture(textureID);

		//Initialize block list
		for (Uint16 b = 0; b < 4; b++) {
			BLOCK_LIST.push_back(Vec2<Uint16>{ 0, 0 });
		}

		BLOCK_LIST[BlockID::GRASS].tile = { 0, 0 };
		BLOCK_LIST[BlockID::DIRT].tile = { 0, 1 };
		BLOCK_LIST[BlockID::STONE].tile = { 0, 2 };

		//Initialize chunks
		ch_ptr = std::make_shared<Chunk>();

		for (Uint32 b = 0; b < CHUNK_AREA; b++) {
			ch_ptr->blocks.push_back({ BlockID::STONE, 0 });
			ch_ptr->lightUpdates.push_back(b);
		}

		/*//Place stone
		for (Uint32 x = 0; x < CHUNK_SIZE; x++) {
			auto& _bl = ch_ptr->blocks[0 * CHUNK_SIZE + x];
			_bl.id = BlockID::STONE;
		}*/

		ch_ptr->autoTile();
		ch_ptr->updateLight();
		Chunks.push_back(std::move(ch_ptr));

	}

	void update(double dt) override final {

	}

	void draw() override final {
		Uint16 tx, ty;
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
					//if (_bl.light == 0) continue;

					switch (_bl.light) {
						case 0: SDL_SetTextureColorMod(texture, 128, 128, 128); break; // 50% light
						default: SDL_SetTextureColorMod(texture, 255, 255, 255); break; // 100 % light
					}

					tr.x = (int)floor((tx * TILE_SIZE) - visibleArea.x);
					tr.y = (int)floor((ty * TILE_SIZE) - visibleArea.y);

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
			if (_tx > CHUNK_SIZE - 1 || _ty > CHUNK_SIZE - 1) return;

			//Render marker
			tr.x = (int)floor((_tx * TILE_SIZE) - visibleArea.x);
			tr.y = (int)floor((_ty * TILE_SIZE) - visibleArea.y);
			SDL_SetRenderDrawColor(rTarget, 86, 214, 255, 255);
			SDL_RenderDrawRect(rTarget, &tr);
			
			/*
			//Only render what is on screen
			for (Sint16 ty = start_y; ty <= end_y; ty++) {
				for (Sint16 tx = start_x; tx <= end_x; tx++) {

					//BOUNDS CHECK
					if  (tx < 0 || ty < 0 || tx > CHUNK_SIZE - 1 || ty > CHUNK_SIZE - 1) continue;

					auto& _bl = ch_ptr->blocks[ty * CHUNK_SIZE + tx];

					SDL_SetRenderDrawColor(rTarget, 128, 128, 128, 255);

					if (_bl.id == BlockID::AIR) continue;
					if (_bl.light == 0) SDL_SetRenderDrawColor(rTarget, 64, 64, 64, 255);

					tr = { (int)floor((tx * TILE_SIZE) - Core::get().getVisibleArea().x), (int)floor((ty * TILE_SIZE) - Core::get().getVisibleArea().y), TILE_SIZE, TILE_SIZE };
					SDL_RenderFillRect(rTarget, &tr);
				}
			}*/
			//Render mouse pos marker
		//dstRect.x = (int)floor(tilemapManager->getMousePosTile().x * TILE_SIZE) - Core::get().getCamera()->x;
		//dstRect.y = (int)floor(tilemapManager->getMousePosTile().y * TILE_SIZE) - Core::get().getCamera()->y;
		//SDL_SetRenderDrawColor(rTarget, 86, 214, 255, 255);
		//SDL_RenderDrawRect(rTarget, &dstRect);
			/*

			for (Uint32 t = 0; t < ch_ptr->blocks.size(); t++) {
				auto& _bl = ch_ptr->blocks[t];

				//if (_bl.id == BlockID::AIR) continue;
				//if (_bl.light == 0) continue;

				tx = (t % CHUNK_SIZE) * TILE_SIZE;
				ty = (t / CHUNK_SIZE) * TILE_SIZE;

				//Camera culling
				tr = { tx - Core::get().getVisibleArea().x, ty - Core::get().getVisibleArea().y, TILE_SIZE, TILE_SIZE };
				cr = Core::get().getVisibleArea();
				cr.x = Core::get().getVisibleArea().x + (Core::get().getVisibleArea().w/2);
				cr.y = Core::get().getVisibleArea().y + (Core::get().getVisibleArea().h/2);

				//std::cout << cr.x << " " << cr.y << " " << cr.w << " " << cr.h << std::endl;
				//std::cout << tr.x << " " << tr.y << " " << tr.w << " " << tr.h << std::endl;
				SDL_RenderDrawRect(rTarget, &cr);

				if (!SDL_HasIntersection(&cr, &tr)) continue;

				if(t == 0)
					SDL_SetRenderDrawColor(rTarget, 0x00, 0, 0x00, 0xFF);
				else
					SDL_SetRenderDrawColor(rTarget, 0x00, 0xFF, 0x00, 0xFF);

				//Draw correct texture clip
				SDL_RenderDrawRect(rTarget, &tr);

			}*/
		}
	}
	
	void drawSpriteClip(SDL_Texture* tex, Uint16 srcX, Uint16 srcY, SDL_Rect* dstRect) {
		SDL_Rect srcRect = { srcX, srcY, TILE_SIZE, TILE_SIZE };
		SDL_RenderCopy(rTarget, texture, &srcRect, dstRect);
	}

private:
	SDL_Renderer* rTarget = nullptr;
	SDL_Texture* texture = nullptr;
	std::string textureID;

	std::vector<BlockInfo> BLOCK_LIST;
	std::vector<std::shared_ptr<Chunk>> Chunks;
	std::shared_ptr<Chunk> ch_ptr;
};