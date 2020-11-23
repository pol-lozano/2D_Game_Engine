#pragma once
#include "Core.h"

class TextureHandler
{
public:
	//Loads image at specified path
	static SDL_Texture* loadFromFile(const char* path);

#if defined(SDL_TTF_MAJOR_VERSION)
	//Creates image from font string
	static SDL_Texture* loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font* font);
#endif
	//Draws texture
	static void draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest,SDL_RendererFlip flip);
};

