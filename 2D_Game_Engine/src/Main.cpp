#include <iostream>
#include "Core/Core.h"
#include "Timer/Time.h"
#include <string>

int main(int argc, char* args[]) {

	Core& core = Core::get();
	core.init();
	Time& time = Time::get();


	while (core.isRunning()) {	
		//Uint64 frameStart = SDL_GetPerformanceCounter();
		core.events();
		core.update(1);
		//time.tick();
		core.render();
		//Uint64 frameEnd = SDL_GetPerformanceCounter();

		//float elapsed = (frameEnd - frameStart) / (float)SDL_GetPerformanceFrequency();
		//std::cout << "FPS: " << std::to_string(1.0f / elapsed) << std::endl;
	}

	core.clean();
	return 0;
}