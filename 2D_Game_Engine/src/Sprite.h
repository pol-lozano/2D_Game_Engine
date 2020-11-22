#pragma once
#include <SDL.h> 
#include "TextureHandler.h"

class Sprite
{
public:
	Sprite(const char* source);
	~Sprite();

	//Deallocates texture
	void free();

	//Set color
	void setColor(Uint8 r, Uint8 g, Uint8 b);

	//Set alpha modulation
	void setAlpha(Uint8 a);

	//Set blend mode
	void setBlendMode(SDL_BlendMode blending);

	//Renders texture at given point, clip selects which part of the texture
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* m_Texture;
	//Image dimensions
	SDL_Point m_Size;

	double m_Rotation = 0;
	SDL_RendererFlip flipType = SDL_FLIP_NONE;		
};

