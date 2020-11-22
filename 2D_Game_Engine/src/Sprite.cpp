#include "Sprite.h"

//Base constructor
Sprite::Sprite(const char* texturePath)
{
	m_Texture = TextureHandler::loadFromFile(texturePath);

	//Set image size
	SDL_QueryTexture(m_Texture, NULL, NULL, &m_Size.x, &m_Size.y);
}

//Destructor
Sprite::~Sprite()
{
	free();
}

void Sprite::free()
{
	//Free texture if it exists
	if (m_Texture != NULL)
	{
		SDL_DestroyTexture(m_Texture);
		m_Texture = NULL;
		m_Size.x = 0;
		m_Size.y = 0;
	}
}

void Sprite::setColor(Uint8 r, Uint8 g, Uint8 b) 
{
	SDL_SetTextureColorMod(m_Texture, r, g, b);
}

void Sprite::setAlpha(Uint8 a)
{
	SDL_SetTextureAlphaMod(m_Texture, a);
}

void Sprite::setBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(m_Texture, blending);
}

void Sprite::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, m_Size.x, m_Size.y };

	//Set clip dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(Core::gRenderer, m_Texture, clip, &renderQuad, angle, center, flip);
}

int Sprite::getWidth()
{
	return m_Size.x;
}

int Sprite::getHeight()
{
	return m_Size.y;
}
