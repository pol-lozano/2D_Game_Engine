#pragma once
#include <SDL.h>

const int FPS_TARGET{ 60 };
const double CYCLE_TIME{ 1.0f / FPS_TARGET };

struct Timer {
	Uint64 previous_ticks{};
	double elapsed_seconds{};

	void tick() {
		const Uint64 current_ticks { SDL_GetPerformanceCounter() };
		const Uint64 delta { current_ticks - previous_ticks };

		previous_ticks = current_ticks;
		static const Uint64 TICKS_PER_SECOND { SDL_GetPerformanceFrequency() };
		elapsed_seconds = delta / static_cast<double>(TICKS_PER_SECOND);

		//Cap delta time
		if (elapsed_seconds > 1) elapsed_seconds = 0.016f;
	}
};