#pragma once
#include <string>
#include <SDL.h>

#include "../Entity.h"
#include "../Component.h"
#include "../../AssetManager/AssetManager.h"

class Sprite : public Component
{
public:
	Sprite() = default;
	virtual ~Sprite() = default;

	Sprite(SDL_Renderer* target, std::string textureid) : rTarget(target), textureID(textureid) { }

	bool init() override final {
		transform = &entity->getComponent<Transform>();
		texture = AssetManager::get().getTexture(textureID);
		SDL_QueryTexture(texture, NULL, NULL, &width, &height);

		dstRect.x = transform->position.x;
		dstRect.y = transform->position.y;
		dstRect.w = width * transform->scale.x;
		dstRect.w = height * transform->scale.y;

		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = width;
		srcRect.h = height;

		return true;
	}

	void draw() override final {
		SDL_RenderCopyEx(rTarget, texture, &srcRect, &dstRect, transform->rotation, NULL, flip);
	}

	void update(float dt) override final {
		dstRect.x = static_cast<int>(transform->position.x);
		dstRect.y = static_cast<int>(transform->position.y);
		dstRect.w = static_cast<int>(width*transform->scale.x);
		dstRect.h = static_cast<int>(height*transform->scale.y);
	}

	inline int getWidth() {
		return width;
	}

	inline int getHeight() {
		return height;
	}

private:
	int width = 0;
	int height = 0;
	SDL_Rect srcRect = { 0, 0, 0, 0 };
	SDL_Rect dstRect = { 0, 0, 0, 0 };

	Transform* transform = nullptr;
	std::string textureID = "";
	SDL_Texture* texture = nullptr;
	SDL_Renderer* rTarget = nullptr;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
};

