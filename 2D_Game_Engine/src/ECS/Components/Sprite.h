#pragma once
#include <string>
#include <SDL.h>

#include "../Component.h"
#include "../../AssetManager/AssetManager.h"

class Sprite : public Component {
public:
	Sprite(SDL_Renderer* target, std::string textureid) : rTarget(target), textureID(textureid) { }

	void init() override final {
		transform = &entity->getComponent<Transform>();
		texture = AssetManager::get().getTexture(textureID);

		//Read texture size
		SDL_QueryTexture(texture, nullptr, nullptr, &size.x, &size.y);
		srcRect = { 0, 0, size.x, size.y };
	}

	void update(double dt) override final {
		const SDL_Rect visibleArea = Core::get().getVisibleArea();

		dstRect.w = static_cast<int>(size.x * transform->scale.x);
		dstRect.h = static_cast<int>(size.y * transform->scale.y);

		dstRect.x = static_cast<int>(transform->position.x) - visibleArea.x;
		dstRect.y = static_cast<int>(transform->position.y) - visibleArea.y;
	}

	void draw() override final { 
		SDL_RenderCopyEx(rTarget, texture, &srcRect, &dstRect, transform->rotation, NULL, flip); 
	}

	inline int getWidth() { return size.x; }
	inline int getHeight() { return size.y; }
	inline Vec2F getCenter() { return Vec2F((size.x * transform->scale.x) / 2, (size.y * transform->scale.y) / 2); }

	//Set color
	inline void setColor(Uint8 r, Uint8 g, Uint8 b) { SDL_SetTextureColorMod(texture, r, g, b); }
	//Set alpha modulation
	inline void setAlpha(Uint8 a) { SDL_SetTextureAlphaMod(texture, a); }
	//Set blend mode
	inline void setBlendMode(SDL_BlendMode mode) { SDL_SetTextureBlendMode(texture, mode); }

private:
	Transform* transform = nullptr;
	SDL_Renderer* rTarget = nullptr;
	SDL_Texture* texture = nullptr;
	std::string textureID = "";

	SDL_Point size = { 0, 0 };
	SDL_Rect srcRect = { 0, 0, 0, 0 };
	SDL_Rect dstRect = { 0, 0, 0, 0 };

	SDL_RendererFlip flip = SDL_RendererFlip::SDL_FLIP_NONE;
};

