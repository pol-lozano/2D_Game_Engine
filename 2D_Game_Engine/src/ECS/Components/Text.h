#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

#include "../Component.h"

class Text : public Component {
public:
	Text(SDL_Renderer* target, int x, int y, std::string txt, std::string fontid) : rTarget(target), position{x,y}, text(txt), fontID(fontid) { }

	void init() override final {
		font = AssetManager::get().getFont(fontID);
		setText(text);
	}

	void update(double dt) override final {
		dstRect.x = position.x;
		dstRect.y = position.y;
	}

	void draw() override final { 
		SDL_RenderCopy(rTarget, texture, nullptr, &dstRect); 
	}

	inline void setText(std::string text) {
		//Clear previous texture
		if (texture != NULL) { SDL_DestroyTexture(texture); }

		//Generate texture from text
		SDL_Surface* surf = TTF_RenderText_Blended(font, text.c_str(), color);
		texture = SDL_CreateTextureFromSurface(rTarget, surf);
		SDL_FreeSurface(surf);

		SDL_QueryTexture(texture, nullptr, nullptr, &dstRect.w, &dstRect.h);
	}

	inline void setFontColor(Uint8 r, Uint8 g, Uint8 b) { color = { r, g, b }; }

private:
	SDL_Point position = { 0, 0 };
	SDL_Renderer* rTarget = nullptr;
	SDL_Texture* texture = nullptr;

	SDL_Rect dstRect = { 0, 0, 0, 0 };

	TTF_Font* font = nullptr;
	std::string fontID = "";

	std::string text = "";
	SDL_Color color = { 255, 255, 255};
};