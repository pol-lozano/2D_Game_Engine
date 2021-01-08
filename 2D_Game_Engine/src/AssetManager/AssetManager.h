#pragma once
#include <map>
#include <SDL.h>
#include <string>
#include <SDL_ttf.h>
#include <SDL_image.h>

class AssetManager
{
public:
	AssetManager();
	~AssetManager() = default;

	//Singleton
	inline static AssetManager& get() {
		if (s_instance == nullptr) {
			s_instance = new AssetManager();
		}
		return *s_instance;
	}

	void loadTexture(std::string id, std::string path);
	SDL_Texture* getTexture(std::string id);

	void loadFont(std::string id, std::string path, int fontSize);
	TTF_Font* getFont(std::string id);

	void clean();

private:
	static AssetManager* s_instance;
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;
};

