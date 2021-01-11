#pragma once
#include "../Component.h"
#include <SDL.h>
#include <string>
#include <iomanip>

constexpr float DEFAULT_SMOOTHING = 15;
class InputHandler : public Component {
public:
	InputHandler(){};
	InputHandler(float smoothing) : m_smoothing(smoothing) {};

	void update(double dt) override final {
		//Get the keystates
		SDL_PumpEvents();
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

		//Y AXIS (Vertical
		auto UP = (currentKeyStates[SDL_SCANCODE_W] | currentKeyStates[SDL_SCANCODE_UP]);
		auto DOWN = (currentKeyStates[SDL_SCANCODE_S] | currentKeyStates[SDL_SCANCODE_DOWN]);
		m_target.y = UP - DOWN;

		//X AXIS (Horizontal)
		auto RIGHT = (currentKeyStates[SDL_SCANCODE_D] | currentKeyStates[SDL_SCANCODE_RIGHT]);
		auto LEFT = (currentKeyStates[SDL_SCANCODE_A] | currentKeyStates[SDL_SCANCODE_LEFT]);
		m_target.x = RIGHT - LEFT;

		//Smoothly lerp current input to target
		m_input.lerp(m_input, m_target, dt * m_smoothing);
	}

	inline Vec2F getInput() { return m_input; }
	inline Vec2F getInputRaw() { return m_target; }

private:
	Vec2F m_input = Vec2F();
	Vec2F m_target = Vec2F();
	float m_smoothing = DEFAULT_SMOOTHING;
};