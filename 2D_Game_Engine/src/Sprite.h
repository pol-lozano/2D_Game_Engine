#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

class Sprite
{
public:
	//Constructor
	Sprite();

	//Destructor
	~Sprite();

	//Deallocates texture
	void free();

	//Loads image at specified path
	bool loadFromFile(std::string path);

	#if defined(SDL_TTF_MAJOR_VERSION)
	//Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font* font);
	#endif

	//Set color and alpha modulation
	void setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	//Set blend mode
	void setBlendMode(SDL_BlendMode blending);

	//Set new clip region
	void setClipRegion(int x, int y, int w, int h);

	//Pick what region gets currently renderered
	void setCurrentClip(int i);
	SDL_Rect* getClip(int i);

	SDL_Rect* getCurrentClip();
	//Renders texture at given point, clip selects which part of the texture
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();

	//Static references to global renderer and font
	static SDL_Renderer* m_Renderer;
	static TTF_Font* m_Font;

private:
	//The actual hardware texture
	SDL_Texture* m_Texture;

	//Spritesheet/animation support
	std::vector<SDL_Rect> m_SpriteClips;
	int m_CurrentClip; 
	
	//Image dimensions
	int m_Width;
	int m_Height;

	/*
	double m_Rotation = 0;
	SDL_RendererFlip flipType = SDL_FLIP_NONE;		
	*/
};

