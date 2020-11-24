#include <iostream>

#include "Core/Core.h"

int main(int argc, char* args[]) {

	Core& core = Core::get();
	core.init();

	const int FPS = 60;
	const int TICKS_PER_FRAME = 1000 / FPS;

	Uint32 frameStart;
	Uint32 countedFrames = 0;
	int frameTime;

	while (core.isRunning()) {

		frameStart = SDL_GetTicks();

		core.events();
		core.update();
		core.render();

		frameTime = SDL_GetTicks() - frameStart;

		//Calculate fps
		float avgFPS = countedFrames / (frameStart / 1000.f);
		std::cout << avgFPS << std::endl;
		++countedFrames;

		//If frame finished early wait
		if (frameTime < TICKS_PER_FRAME)
			SDL_Delay(TICKS_PER_FRAME - frameTime);
	}

	core.clean();
	return 0;
}