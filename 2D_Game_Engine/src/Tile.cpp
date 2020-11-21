#include "Tile.h"
#include "Sprite.h"
#include "physics/Collision.h"

Tile::Tile(int x, int y, int tileType, Sprite* graphics)
{
    m_Sprite = graphics;
    //Get the offsets
    m_Box.x = x;
    m_Box.y = y;

    //Set the collision box
    m_Box.w = TILE_WIDTH;
    m_Box.h = TILE_HEIGHT;

    //Get the tile type
    m_Type = tileType;
}

void Tile::render(SDL_Rect& camera)
{
    //If the tile is on screen
    if (Collision::checkCollision(camera, m_Box))
    {
        //Show the tile
        m_Sprite->render(m_Box.x - camera.x, m_Box.y - camera.y, m_Sprite->getClip(m_Type));
    }
}

int Tile::getType()
{
    return m_Type;
}

SDL_Rect Tile::getBox()
{
    return m_Box;
}