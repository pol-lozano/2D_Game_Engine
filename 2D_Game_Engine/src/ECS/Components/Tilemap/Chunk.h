#pragma once
#include <SDL.h>
#include <vector>
#include <algorithm>
#include <random>
#include "../../Component.h"

enum BlockID : Uint16 {
	AIR,
	GRASS,
	DIRT,
	STONE,
	WOOD,
	STONEBRICK,
	COPPER,
	IRON,
	SILVER
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
	Uint16 currentTile = 1;

	inline void updateLight() {
		//Get mouse position
		int MOUSE_X, MOUSE_Y;
		int MOUSE_STATE = SDL_GetMouseState(&MOUSE_X, &MOUSE_Y);

		//Translate screen space to tile space
		Uint16 _tx = floor(Core::get().camToWorldX(MOUSE_X) / TILE_SIZE);
		Uint16 _ty = floor(Core::get().camToWorldY(MOUSE_Y) / TILE_SIZE);

		//Get user input to select current tile
		getCurrentTile();

		//Bounds check
		if (_tx < CHUNK_SIZE && _ty < CHUNK_SIZE) {
			//Handle Mouse input
			if (MOUSE_STATE == SDL_BUTTON(SDL_BUTTON_LEFT) || MOUSE_STATE == SDL_BUTTON(SDL_BUTTON_RIGHT) || MOUSE_STATE == SDL_BUTTON(SDL_BUTTON_MIDDLE)) {
				//Get current block
				auto& bl = blocks[std::clamp(_ty * CHUNK_SIZE + _tx, 0u, CHUNK_AREA - 1)];
				//Place or remove blocks
				switch (MOUSE_STATE) {
					//Place block if there is no block
					case SDL_BUTTON(SDL_BUTTON_LEFT): if (bl.id == 0) bl.id = static_cast<BlockID>(currentTile); break;
					//Remove block if there is a block
					case SDL_BUTTON(SDL_BUTTON_RIGHT): if (bl.id != 0) bl.id = BlockID::AIR; break;
					//Copy whatever block you middle click on
					case SDL_BUTTON(SDL_BUTTON_MIDDLE): if (bl.id != 0) currentTile = bl.id; break;
				}

				//Push to light update queue
				//Update self
				lightUpdates.push_back(std::clamp(_ty * CHUNK_SIZE + _tx, 0u, CHUNK_AREA - 1));
				//Sides
				lightUpdates.push_back(std::clamp(_ty * CHUNK_SIZE + (_tx + 1), 0u, CHUNK_AREA - 1));
				lightUpdates.push_back(std::clamp(_ty * CHUNK_SIZE + (_tx - 1), 0u, CHUNK_AREA - 1));
				lightUpdates.push_back(std::clamp((_ty + 1) * CHUNK_SIZE + (_tx), 0u, CHUNK_AREA - 1));
				lightUpdates.push_back(std::clamp((_ty - 1) * CHUNK_SIZE + (_tx), 0u, CHUNK_AREA - 1));
				//Corners
				lightUpdates.push_back(std::clamp((_ty - 1) * CHUNK_SIZE + (_tx - 1), 0u, CHUNK_AREA - 1));
				lightUpdates.push_back(std::clamp((_ty - 1) * CHUNK_SIZE + (_tx + 1), 0u, CHUNK_AREA - 1));
				lightUpdates.push_back(std::clamp((_ty + 1) * CHUNK_SIZE + (_tx - 1), 0u, CHUNK_AREA - 1));
				lightUpdates.push_back(std::clamp((_ty + 1) * CHUNK_SIZE + (_tx + 1), 0u, CHUNK_AREA - 1));
			}

			if (!lightUpdates.empty()) {
				Uint32 lu_s = lightUpdates.size();
				Uint32 tile;
				Uint8 tl, tr, tt, tb;
				Uint8 ttl, ttr, tbl, tbr;

				//Update tiles in update queue
				for (Uint32 l = 0; l < lu_s; l++) {
					tile = lightUpdates.back();
					lightUpdates.pop_back();
					//Sides
					tl = (blocks[std::clamp((Sint32)tile - 1, 0, (Sint32)CHUNK_AREA - 1)].id != BlockID::AIR);
					tr = (blocks[std::clamp((Sint32)tile + 1, 0, (Sint32)CHUNK_AREA - 1)].id != BlockID::AIR);
					tt = (blocks[std::clamp((Sint32)tile - (Sint32)CHUNK_SIZE, 0, (Sint32)CHUNK_AREA - 1)].id != BlockID::AIR);
					tb = (blocks[std::clamp((Sint32)tile + (Sint32)CHUNK_SIZE, 0, (Sint32)CHUNK_AREA - 1)].id != BlockID::AIR);
					//Corners
					ttl = (blocks[std::clamp(((Sint32)tile - (Sint32)CHUNK_SIZE) - 1, 0, (Sint32)CHUNK_AREA - 1)].id != BlockID::AIR);
					ttr = (blocks[std::clamp(((Sint32)tile - (Sint32)CHUNK_SIZE) + 1, 0, (Sint32)CHUNK_AREA - 1)].id != BlockID::AIR);
					tbl = (blocks[std::clamp(((Sint32)tile + (Sint32)CHUNK_SIZE) - 1, 0, (Sint32)CHUNK_AREA - 1)].id != BlockID::AIR);
					tbr = (blocks[std::clamp(((Sint32)tile + (Sint32)CHUNK_SIZE) + 1, 0, (Sint32)CHUNK_AREA - 1)].id != BlockID::AIR);

					//Autotile and set lights
					auto& bl = blocks[tile];
					bl.light = static_cast<Uint8>(!tl + !tr + !tt + !tb + !ttl + !ttr + !tbl + !tbr);
					if (bl.id != BlockID::AIR) bl.sub = static_cast<Uint8>((tt + (tr << 1) + (tb << 2) + (tl << 3)));
				}
			}
		}
	}

	//Autotile chunk
	inline void autoTile() {
		Uint32 tile;
		Uint8 tl, tr, tt, tb;

		for (Uint16 y = 1; y < CHUNK_SIZE - 1; y++) {
			for (Uint16 x = 1; x < CHUNK_SIZE - 1; x++) {
				//get tile position
				tile = y * CHUNK_SIZE + x;
				auto& bl = blocks[tile];

				//Skip empty positions
				if (bl.id == BlockID::AIR) continue;

				tl = (blocks[tile - 1].id != BlockID::AIR);
				tr = (blocks[tile + 1].id != BlockID::AIR);
				tt = (blocks[tile - CHUNK_SIZE].id != BlockID::AIR);
				tb = (blocks[tile + CHUNK_SIZE].id != BlockID::AIR);

				//Calculate subvalue with bitshifting
				bl.sub = static_cast<Uint8>(tt + (tr << 1) + (tb << 2) + (tl << 3));
			}
		}
	}

	inline void generate() {
		randomStep();
		cellularAutomata(5);
		placeGrass(15);
	}

	inline void randomStep() {
		Uint16 lastHeight = CHUNK_SIZE / 2;

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<>distr(-1, 1);

		//Basic random step generation
		for (Uint32 x = 0; x < CHUNK_SIZE; x++) {
			lastHeight += distr(gen);
			for (Uint32 y = 0; y < CHUNK_SIZE; y++) {
				if (y < lastHeight) continue;
				blocks[y * CHUNK_SIZE + x].id = BlockID::STONE;
			}
		}
	}

	inline void cellularAutomata(Uint8 iterations) {
		randomFill("");
		for (Uint8 i = 0; i < iterations; i++) {
			smoothMap();
		}
	}

	inline void randomFill(std::string seed) {
		//Initialize seed with hashcode or random seed
		seed.empty() ? srand(time(NULL)) : srand(std::hash<std::string>{}(seed));

		for (int x = 1; x < CHUNK_SIZE - 1; x++) {
			for (int y = CHUNK_SIZE / 2 + 16; y < CHUNK_SIZE - 1; y++) {
				if (x < 0 || y < 0 || x > CHUNK_SIZE - 1 || y > CHUNK_SIZE - 1) continue;
				if (x == 0 || x == CHUNK_SIZE - 1 || y == 0 || y == CHUNK_SIZE - 1)
					blocks[y * CHUNK_SIZE + x].id = BlockID::STONE;
				else {
					auto value = (rand() % 100 < 50) ? BlockID::STONE : BlockID::AIR;
					blocks[y * CHUNK_SIZE + x].id = value;
				}
			}
		}
	}

	inline void smoothMap() {
		for (int x = 1; x < CHUNK_SIZE - 1; x++) {
			for (int y = CHUNK_SIZE / 2 + 16; y < CHUNK_SIZE - 1; y++) {
				int neighbouringTiles = getSurroundingWallCount(x, y);
				auto& _bl = blocks[y * CHUNK_SIZE + x];

				if (neighbouringTiles > 4)
					_bl.id = BlockID::STONE;
				else if (neighbouringTiles < 4)		
					_bl.id = BlockID::AIR;
			}
		}
	}

	inline int getSurroundingWallCount(Uint16 x, Uint16 y) {
		Uint8 wallCount = 0;
		for (Uint16 neighbourX = x - 1; neighbourX <= x + 1; neighbourX++) {
			for (Uint16 neighbourY = y - 1; neighbourY <= y + 1; neighbourY++) {
				if (neighbourX >= 0 && neighbourX < CHUNK_SIZE && neighbourY >= 0 && neighbourY < CHUNK_SIZE) {
					if (neighbourX != x || neighbourY != y) {
						wallCount += blocks[neighbourY * CHUNK_SIZE + neighbourX].id != BlockID::AIR;
					}
				}
				else {
					//Out of bounds
					wallCount++;
				}
			}
		}
		return wallCount;
	}

	inline void placeGrass(Uint16 depth) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<>distr(7, 9);

		for (Uint32 x = 0; x < CHUNK_SIZE; x++) {
			Uint16 h = getHighestPoint(x);
			for (Uint16 y = h; y < h + distr(gen); y++) {
				if (blocks[std::clamp(y * CHUNK_SIZE + x, 0u, CHUNK_AREA - 1)].id != BlockID::AIR)
					blocks[std::clamp(y * CHUNK_SIZE + x, 0u, CHUNK_AREA - 1)].id = BlockID::GRASS;
			}

			std::uniform_int_distribution<>distr2(0, 20);

			//Place some ore randomly too
			if (distr(gen) % 2) {
				for (Uint16 y = h + 20; y < h + 20 + distr2(gen); y++) {
					if (blocks[std::clamp(y * CHUNK_SIZE + x, 0u, CHUNK_AREA - 1)].id != BlockID::AIR) {
						blocks[std::clamp(y * CHUNK_SIZE + x, 0u, CHUNK_AREA - 1)].id = BlockID::IRON;
						blocks[std::clamp(y * CHUNK_SIZE + (x + 1), 0u, CHUNK_AREA - 1)].id = BlockID::IRON;
					}
				}

				for (Uint16 y = h + 60; y < h + 60 + distr2(gen); y++) {
					if (blocks[std::clamp(y * CHUNK_SIZE + x, 0u, CHUNK_AREA - 1)].id != BlockID::AIR) {
						blocks[std::clamp(y * CHUNK_SIZE + x, 0u, CHUNK_AREA - 1)].id = BlockID::SILVER;
						blocks[std::clamp(y * CHUNK_SIZE + (x + 1), 0u, CHUNK_AREA - 1)].id = BlockID::SILVER;
					}
				}

				for (Uint16 y = h + 20; y < h + 100 + distr2(gen); y++) {
					if (blocks[std::clamp(y * CHUNK_SIZE + x, 0u, CHUNK_AREA - 1)].id != BlockID::AIR) {
						blocks[std::clamp(y * CHUNK_SIZE + x, 0u, CHUNK_AREA - 1)].id = (rand() % 100 < 75) ? blocks[std::clamp(y * CHUNK_SIZE + x, 0u, CHUNK_AREA - 1)].id : (rand() % 100 < 90) ? BlockID::SILVER : BlockID::COPPER;
					}
				}
			}
		}
	}

	//Used for spawning the player in a proper location
	inline int getHighestPoint(Uint16 tx) {
		if (tx < 0 || tx > CHUNK_SIZE - 1) return 0;
		for (Uint32 ty = 0; ty < CHUNK_SIZE; ty++) {
			auto& _bl = blocks[ty * CHUNK_SIZE + tx];
			if (_bl.id != 0) return ty;
		}
		return CHUNK_SIZE;
	}

	inline void getCurrentTile() {
		SDL_PumpEvents();
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

		//Change current tile
		if (currentKeyStates[SDL_SCANCODE_1])
			currentTile = 1;
		if (currentKeyStates[SDL_SCANCODE_2])
			currentTile = 2;
		if (currentKeyStates[SDL_SCANCODE_3])
			currentTile = 3;
		if (currentKeyStates[SDL_SCANCODE_4])
			currentTile = 4;
		if (currentKeyStates[SDL_SCANCODE_5])
			currentTile = 5;
		if (currentKeyStates[SDL_SCANCODE_6])
			currentTile = 6;
		if (currentKeyStates[SDL_SCANCODE_7])
			currentTile = 7;
		if (currentKeyStates[SDL_SCANCODE_8])
			currentTile = 8;
	}
};