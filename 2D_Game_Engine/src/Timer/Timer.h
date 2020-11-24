#pragma once
#include <SDL.h>

class Timer
{
public:
	//Constructor
	Timer();

	void start();
	void stop();
	void pause();
	void unpause();

	//Gets the timer's time
	Uint32 getTicks();

	//Checks the status of the timer
	bool isStarted();
	bool isPaused();
private:
	//Time when timer started
	Uint32 m_StartTicks;
	//Ticks stored when timer was paused
	Uint32 m_PausedTicks;
	//Status
	bool m_Paused;
	bool m_Started;
};

