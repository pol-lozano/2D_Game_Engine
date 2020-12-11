#pragma once
#include <SDL.h>
#include <memory>
#include <string>
#include <vector>
#include "TilemapLayer.h"
#include <iostream>

class Tilemap
{
public:
	Tilemap(size_t w, size_t h) : width(w), height(h) {
		//add base layer which is autotiled
		addLayer(true);
		//Generate base layer
		generate(0);
		//Autotile every layer
		autoTileAll();
	}

	void generate(int layer);
	void randomFill(int layer);
	void smoothMap(int layer);

	int getSurroundingWallCount(int x, int y, int layer);

	inline size_t getWidth() { return width; }
	inline size_t getHeight() { return height; }

	void addLayer(bool autoTiled) {
		layers.emplace_back(TilemapLayer(getWidth(), getHeight(), autoTiled));
	}

	void removeLayer() {
		//remove last element
		if(!layers.empty())
			layers.pop_back();
	}

	inline size_t getLayerCount() { return layers.size(); }

	inline uint8_t getTileValue(int x, int y, int layer) { return layers[layer].getTileValue(getTileID(x, y)); }
	inline uint8_t getTileSubValue(int x, int y, int layer) { return layers[layer].getTileSubValue(getTileID(x, y)); }

	inline void setTile(int x, int y, int layer, uint8_t value, uint8_t subValue) {
		layers[layer].setTile(getTileID(x, y), value, subValue);
	}

	inline uint8_t getDecorationValue(int x, int y, int layer) { return layers[layer].getDecorationValue(getTileID(x, y)); }
	inline uint8_t getDecorationSubValue(int x, int y, int layer) { return layers[layer].getDecorationSubValue(getTileID(x, y)); }

	inline void setDecoration(int x, int y, int layer, uint8_t value, uint8_t subValue) {
		layers[layer].setDecoration(getTileID(x, y), value, subValue);
	}

	void autoTileAll();
	void autoTilePoint(int x, int y,int layer);
	void autoTileNeighbours(int x, int y, int layer);

	inline bool outOfBounds(int x, int y) {
		return (x < 0 || y < 0 || x > width-1 || y > height-1);
	}

	//Gets tile id by x and y pos
	inline int getTileID(int x, int y) {
		if (x < 0 || y < 0) std::cout << x << "/" << y << std::endl;
		return (x * height) + y;
	}

private:
	size_t width, height;
	std::vector<TilemapLayer> layers;

	std::string seed = "";
	bool randomSeed = true;
	int randomFillPercent = 48;
};

