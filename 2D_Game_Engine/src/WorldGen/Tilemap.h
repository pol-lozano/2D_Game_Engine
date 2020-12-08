#pragma once
#include <SDL.h>
#include <memory>
#include <string>

class Tilemap
{
public:
	Tilemap(size_t w, size_t h) : width(w), height(h), data{ std::make_unique<int[]>(w * h) } {}

	void generate();
	void randomFill();
	void smoothMap();

	int getSurroundingWallCount(int x, int y);

	inline size_t getWidth() { return width; }
	inline size_t getHeight() { return height; }

	int* operator[](int x) { 
		return x * width + data.get();
	}

	int& operator()(int x, int y) {
		return data[x * height + y];
	}

private:
	size_t width;
	size_t height;
	std::unique_ptr<int[]> data;

	std::string seed = "";
	bool randomSeed = true;
	int randomFillPercent = 48;
};

