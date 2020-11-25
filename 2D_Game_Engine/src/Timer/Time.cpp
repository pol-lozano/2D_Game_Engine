#include "Time.h"
#include <iostream>;

Time* Time::s_instance = nullptr;

void Time::tick()
{
    // Wait until ~17ms has elapsed since last frame 1 sec /60 fps = 16.666ms per frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_ticksCount + 1/FPS_TARGET));

    // Delta time is the difference in ticks from last frame
    // (converted to seconds)
    m_deltaTime = (SDL_GetTicks() - m_ticksCount) / 1000.0f;

    // Clamp maximum delta time value
    if (m_deltaTime > 0.05f) { m_deltaTime = 0.05f; }
    m_ticksCount = SDL_GetTicks();
}
