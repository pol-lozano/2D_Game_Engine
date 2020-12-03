#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

#include "../Entity.h"
#include "../Component.h"

class Text : public Component {
public:
	Text() = default;
	~Text() = default;

	Text(SDL_Renderer* target, std::string txt, std::string fontid) : rTarget(target), text(txt), fontID(fontid) { }

	bool init() override final {
		transform = &entity->getComponent<Transform>();
		font = AssetManager::get().getFont(fontID);
		setText(text);
		return true;
	}

	void update(float dt) override final {
		dstRect.w = static_cast<int>(size.x);
		dstRect.h = static_cast<int>(size.y);

		dstRect.x = static_cast<int>(10);
		dstRect.y = static_cast<int>(10);
	}

	void draw() override final {
		SDL_RenderCopy(rTarget, texture, nullptr, &dstRect);
	}

	inline void setText(std::string text) {
		//Clear previous texture
		SDL_DestroyTexture(texture);

		SDL_Surface* surf = TTF_RenderText_Blended(font, text.c_str(), color);
		texture = SDL_CreateTextureFromSurface(rTarget, surf);
		SDL_FreeSurface(surf);
		SDL_QueryTexture(texture, nullptr, nullptr, &size.x, &size.y);
	}

	inline void setFontColor(Uint8 r, Uint8 g, Uint8 b) {
		color = { r, g, b };
	}

private:
	Transform* transform = nullptr;
	SDL_Renderer* rTarget = nullptr;
	SDL_Texture* texture = nullptr;

	SDL_Point size = { 0, 0 };
	SDL_Rect dstRect = { 0, 0, 0, 0 };

	TTF_Font* font = nullptr;
	std::string fontID = "";

	std::string text = "";
	SDL_Color color = { 255, 255, 255};
};