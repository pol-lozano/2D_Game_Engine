#pragma once
#include <memory>

class TilemapLayer
{
public:
	TilemapLayer(size_t w, size_t h, bool autoTiled) : width(w), height(h), autoTile(autoTiled)
	{
		tileValues = std::make_unique<uint8_t[]>(w * h);
		decorationValues = std::make_unique<uint8_t[]>(w * h);
		tileSubValues = std::make_unique<uint8_t[]>(w * h);
		decorationSubValues = std::make_unique<uint8_t[]>(w * h);
	}

	inline uint8_t getTileValue(int id) { return tileValues[id]; }
	inline uint8_t getTileSubValue(int id) { return tileSubValues[id]; }

	//Set tile value and subvalue by id
	inline void setTile(int id, uint8_t value, uint8_t subValue) {
		tileValues[id] = value;
		tileSubValues[id] = subValue;
	}

	inline uint8_t getDecorationValue(int id) { return decorationValues[id]; }
	inline uint8_t getDecorationSubValue(int id) { return decorationSubValues[id]; }

	//Set decoration value and subvalue by id
	inline void setDecoration(int id, uint8_t value, uint8_t subValue) {
		decorationValues[id] = value;
		decorationSubValues[id] = subValue;
	}

	inline bool isAutoTiled(){ return autoTile; }

private:
	size_t width = 0;
	size_t height = 0;

	//Auto bitmask layer?
	bool autoTile;

	//tilesheet values
	std::unique_ptr<uint8_t[]> tileValues;
	std::unique_ptr<uint8_t[]> decorationValues;

	//Tile values
	std::unique_ptr<uint8_t[]> tileSubValues;
	std::unique_ptr<uint8_t[]> decorationSubValues;
};

