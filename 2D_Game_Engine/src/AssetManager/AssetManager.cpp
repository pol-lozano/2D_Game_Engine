#include <iostream>
#include "AssetManager.h"
#include "../Core/Core.h"

AssetManager* AssetManager::s_instance = nullptr;

AssetManager::AssetManager(){
    if (TTF_Init() != 0)
        std::cerr << TTF_GetError() << std::endl;
    if(IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG) != 0)
        std::cerr << TTF_GetError() << std::endl;
}

//Load texture from path and store it in a map
void AssetManager::loadTexture(std::string id, std::string path) {
    if (textures.count(id) <= 0) {
        SDL_Texture* texture = IMG_LoadTexture(Core::get().getRenderer(), path.c_str());
        if (texture) {
            textures[id] = texture;
            printf("Texture: [%s] successfully loaded!\n", path.c_str());
        }
        else 
            std::cerr << IMG_GetError() << std::endl;
    }
}

//Retrieve image from map by string id
SDL_Texture* AssetManager::getTexture(std::string id) { return (textures.count(id) > 0) ? textures[id] : nullptr; }

//Load front from map and store it in map
void AssetManager::loadFont(std::string id, std::string path, int fontSize) {
    TTF_Font* newfont = TTF_OpenFont(path.c_str(), fontSize);
    if (newfont != nullptr) {
        fonts.emplace(id, newfont);
        printf("Font: [%s] successfully loaded!\n", path.c_str());
    }
    else 
        std::cerr << TTF_GetError() << std::endl;
}

//Retrieve font from map by string id
TTF_Font* AssetManager::getFont(std::string id) { return (fonts.count(id) > 0) ? fonts[id] : nullptr; }

void AssetManager::clean() {
    //Clear all textures
    for (auto it = textures.begin(); it != textures.end();) {
        SDL_DestroyTexture(it->second);
        textures.erase(it++);
    }

    textures.clear();

    //Clear all fonts
    for (auto it = fonts.begin(); it != fonts.end();) {
        TTF_CloseFont(it->second);
        fonts.erase(it++);
    }

    fonts.clear();

    IMG_Quit();
    TTF_Quit();

    printf("Assets successfully cleaned!\n");
}


