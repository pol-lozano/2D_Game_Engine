#include "Entity.h"
#include "physics/Collision.h"

Entity::Entity(Sprite* graphics)
{
    m_Graphics = graphics;
    m_Box.x = 0;
    m_Box.y = 0;
    m_Box.w = m_Graphics->getWidth();
    m_Box.h = m_Graphics->getHeight();
	m_VelX = 0;
	m_VelY = 0;
}

void Entity::handleEvent(SDL_Event& e)
{
    //If a key was pressed
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_UP: m_VelY -= m_Speed; break;
        case SDLK_DOWN: m_VelY += m_Speed; break;
        case SDLK_LEFT: m_VelX -= m_Speed; break;
        case SDLK_RIGHT: m_VelX += m_Speed; break;
        }
    }
    //If a key was released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_UP: m_VelY += m_Speed; break;
        case SDLK_DOWN: m_VelY -= m_Speed; break;
        case SDLK_LEFT: m_VelX += m_Speed; break;
        case SDLK_RIGHT: m_VelX -= m_Speed; break;
        }
    }
}

void Entity::move(Tile* tiles[], int w, int h, int tilemap_Size)
{
    //Move the dot left or right
    m_Box.x += m_VelX;

    //If the dot went too far to the left or right or touched a wall
    if ((m_Box.x < 0) || (m_Box.x + m_Box.w > w) || Collision::touchesWall(m_Box, tiles, tilemap_Size))
    {
        //move back
        m_Box.x -= m_VelX;
    }

    //Move the dot up or down
    m_Box.y += m_VelY;

    //If the dot went too far up or down or touched a wall
    if ((m_Box.y < 0) || (m_Box.y + m_Box.h > h) || Collision::touchesWall(m_Box, tiles, tilemap_Size))
    {
        //move back
        m_Box.y -= m_VelY;
    }
}

void Entity::setCamera(SDL_Rect& camera, int w, int h, int levelWidth, int levelHeight)
{ 
        //Center the camera over the entity
        camera.x = (m_Box.x + m_Box.w / 2) - w / 2;
        camera.y = (m_Box.y + m_Box.h / 2) - h / 2;

        //Keep the camera in bounds
        if (camera.x < 0)
        {
            camera.x = 0;
        }
        if (camera.y < 0)
        {
            camera.y = 0;
        }
        if (camera.x > levelWidth - camera.w)
        {
            camera.x = levelWidth - camera.w;
        }
        if (camera.y > levelHeight - camera.h)
        {
            camera.y = levelHeight - camera.h;
        }
}

void Entity::render(SDL_Rect& camera)
{
    m_Graphics->render(m_Box.x - camera.x, m_Box.y - camera.y);
}
