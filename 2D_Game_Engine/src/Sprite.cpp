#include "Sprite.h"

//Static references to global renderer
SDL_Renderer* Sprite::m_Renderer = NULL;

//Base constructor
Sprite::Sprite()
{
	m_CurrentClip = 0;
	m_Texture = NULL;
	m_Width = 0;
	m_Height = 0;
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
		m_CurrentClip = 0;
		m_SpriteClips.clear();
		m_Texture = NULL;
		m_Width = 0;
		m_Height = 0;
	}
}

bool Sprite::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(m_Renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			m_Width = loadedSurface->w;
			m_Height = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	m_Texture = newTexture;
	return m_Texture != NULL;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool Sprite::loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font* font)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
	if (textSurface != NULL)
	{
		//Create texture from surface pixels
		m_Texture = SDL_CreateTextureFromSurface(m_Renderer, textSurface);
		if (m_Texture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			m_Width = textSurface->w;
			m_Height = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	else
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}

	//Return success
	return m_Texture != NULL;
}
#endif

void Sprite::setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) 
{
	SDL_SetTextureColorMod(m_Texture, r, g, b);
	SDL_SetTextureAlphaMod(m_Texture, a);
}

void Sprite::setBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(m_Texture, blending);
}

void Sprite::setClipRegion(int x, int y, int w, int h)
{
	//Define clip region
	SDL_Rect clipQuad = { x, y, w, h };
	//Store definition
	m_SpriteClips.emplace_back(clipQuad);
}

void Sprite::setCurrentClip(int i)
{
	if (i <= m_SpriteClips.size())
		m_CurrentClip = i;
}

SDL_Rect* Sprite::getClip(int i) {
	if (i <= m_SpriteClips.size())
		return &m_SpriteClips[i];
	else return NULL;
}

SDL_Rect* Sprite::getCurrentClip()
{
	if (!m_SpriteClips.empty())
		return &m_SpriteClips[m_CurrentClip];
	else return NULL;
}

void Sprite::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, m_Width, m_Height };

	//Set clip dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(m_Renderer, m_Texture, clip, &renderQuad, angle, center, flip);
}

int Sprite::getWidth()
{
	return m_Width;
}

int Sprite::getHeight()
{
	return m_Height;
}
