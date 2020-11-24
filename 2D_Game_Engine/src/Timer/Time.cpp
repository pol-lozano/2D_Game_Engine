#include "Time.h"

Time* Time::s_instance = nullptr;

Time::Time(){}

Time::~Time(){}

void Time::tick()
{
    // Wait until 16ms has elapsed since last frame 60 fps = 16ms per frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_ticksCount + 16));

    // Delta time is the difference in ticks from last frame
    // (converted to seconds)
    m_deltaTime = (SDL_GetTicks() - m_ticksCount) / 1000.0f;

    // Clamp maximum delta time value
    if (m_deltaTime > 0.05f) { m_deltaTime = 0.05f; }
}
