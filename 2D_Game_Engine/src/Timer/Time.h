#pragma once
#include <SDL.h>

class Time {
public:
	Time();
	~Time();

	void tick();

	inline float getDeltatime() { return m_deltaTime; }

	//Singleton
	inline static Time& get() {
		if (s_instance == nullptr) {
			s_instance = new Time();
		}
		return *s_instance;
	}
	
private:
	static Time* s_instance;
	float m_deltaTime = 0;
	Uint32 m_ticksCount = 0;
};