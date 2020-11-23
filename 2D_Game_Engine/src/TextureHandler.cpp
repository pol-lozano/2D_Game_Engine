#include "TextureHandler.h"

SDL_Texture* TextureHandler::loadFromFile(const char* path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path);
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(Core::gRenderer, loadedSurface);

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

#if defined(SDL_TTF_MAJOR_VERSION)
SDL_Texture* TextureHandler::loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font* font)
{
		//The final texture
		SDL_Texture* newTexture = NULL;
		//Render text surface
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
		if (textSurface == NULL)
		{
			printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
		}
		else
		{
			//Create texture from surface pixels
			newTexture = SDL_CreateTextureFromSurface(Core::gRenderer, textSurface);
	
			//Get rid of old surface
			SDL_FreeSurface(textSurface);
		}

		return newTexture;
}
#endif

void TextureHandler::draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip)
{
	SDL_RenderCopyEx(Core::gRenderer, tex, &src, &dest,NULL,NULL,flip);
}

