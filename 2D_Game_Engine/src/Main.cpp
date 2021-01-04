#include "Core/Core.h"
#include "Timer/Timer.h"

int main(int argc, char* args[]) {

	Core& core = Core::get();
	core.init();

	// System timing
	static Timer system_timer;
	double accumulated_seconds{ 0.0f };

	while (core.isRunning()) {
		//Update system clock
		system_timer.tick();
		accumulated_seconds += system_timer.elapsed_seconds;

		//Event loop
		core.events();

		// Cap the framerate
		while (std::isgreater(accumulated_seconds, CYCLE_TIME)) {
			// Reset the accumulator
			accumulated_seconds = 0.0f;

			//Physics loop
			static Timer physics_timer;
			physics_timer.tick();
			core.update(physics_timer.elapsed_seconds);

			//Render loop
			core.render();
		}
	}

	core.clean();
	return 0;
}