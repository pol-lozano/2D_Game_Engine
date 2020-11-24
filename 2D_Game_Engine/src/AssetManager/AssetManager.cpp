#include "AssetManager.h"
#include <iostream>
#include "../Core/Core.h"

AssetManager* AssetManager::s_instance = nullptr;

AssetManager::AssetManager(){
    if (TTF_Init() != 0)
        std::cerr << TTF_GetError() << std::endl;
    if(IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG) != 0)
        std::cerr << TTF_GetError() << std::endl;
}

void AssetManager::loadTexture(std::string id, std::string path){
    if (textures.count(id) <= 0) {
        SDL_Texture* texture = IMG_LoadTexture(Core::get().getRenderer(), path.c_str());
        if (texture) {
            textures[id] = texture;
            std::cout << "texture: [" << path << "] successfully loaded!" << std::endl;
        }
        else
            std::cerr << IMG_GetError() << std::endl;
    }
}

SDL_Texture* AssetManager::getTexture(std::string id) {
    return (textures.count(id) > 0) ? textures[id] : nullptr;
}

void AssetManager::loadFont(std::string id, std::string path, int fontSize){
    TTF_Font* newfont = TTF_OpenFont(path.c_str(), fontSize);
    if (newfont != nullptr) {
        fonts.emplace(id, newfont);
        std::cout << "font: [" << path << "] successfully loaded!" << std::endl;
    }
    else
        std::cerr << TTF_GetError() << std::endl;
}

TTF_Font* AssetManager::getFont(std::string id) {
    return (fonts.count(id) > 0) ? fonts[id] : nullptr;
}

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

    std::cout << "Assets cleaned successfully!" << std::endl;
    
    IMG_Quit();
    TTF_Quit();
}


