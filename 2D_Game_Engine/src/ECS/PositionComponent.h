#pragma once
#include "Components.h"

class PositionComponent : public Component {
private:
	int xPos, yPos;
public:
	PositionComponent() {
		xPos = 0;
		xPos = 0;
	}

	PositionComponent(int x, int y) {
		xPos = x;
		yPos = y;
	}

	void update() override
	{
		xPos++;
		yPos++;
	}

	int x() { return xPos; }
	void x(int x) { xPos = x; }
	int y() { return yPos; }
	void y(int y) { yPos = y; }
	void setPos(int x, int y) { xPos = x; yPos = y; }
};