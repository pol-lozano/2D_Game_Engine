#pragma once
#include <SDL.h>
#include <string>
#include "../Entity.h"
#include "../Component.h"
#include "../../Core/Core.h"
#include <iomanip>

class InputHandler : public Component {
public:
	bool init() override final {
		m_input.zero();
		m_target.zero();
		m_smoothing = 10;
		return true;
	}

	void update(float dt) override final {
		//Get the keystates
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

		//Y AXIS (Vertical)
		if (currentKeyStates[SDL_SCANCODE_UP] || currentKeyStates[SDL_SCANCODE_DOWN]) {
			m_target.y = (currentKeyStates[SDL_SCANCODE_UP]) ? 1 : -1;
		}
		else{ m_target.y = 0; }

		//X AXIS (Horizontal)
		if (currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_LEFT]) {
			m_target.x = (currentKeyStates[SDL_SCANCODE_RIGHT]) ? 1 : -1;
		}
		else{ m_target.x = 0; }

		//Smoothly lerp current input to target
		m_input.lerp(m_input, m_target, dt * m_smoothing);
		std::cout << m_input << std::endl;
	}

	inline Vector2 getInput() {		
		return m_input;
	}

private:
	Vector2 m_input;
	Vector2 m_target;
	float m_smoothing;
};