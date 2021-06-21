//
//  sdllibrary.cpp
//  RPG
//
//  Created by Иван Гаврилов on 05.11.2019.
//  Copyright © 2019 Иван Гаврилов. All rights reserved.
//

#include "sdllibrary.hpp"
SDL_Texture* LoadImage(SDL_Renderer *ren, const std::string &path) {
    SDL_Texture *tex = IMG_LoadTexture(ren, path.c_str());
    if (tex == nullptr)
        std::cout << "LoadTexture Error!";
    return tex;
}
void renderTexture(SDL_Renderer *ren, SDL_Texture *tex, int x, int y, int w, int h, SDL_RendererFlip flip) {
    SDL_Rect rec = {x, y, w, h};
    SDL_RenderCopyEx(ren, tex, NULL, &rec, 0.0, NULL, flip);
}
void renderTexture(SDL_Renderer *ren, SDL_Texture *tex, int x, int y, SDL_RendererFlip flip) {
    int w,h;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    renderTexture(ren, tex, x, y, w, h, flip);
}
std::vector<SDL_Texture*> LoadImages(SDL_Renderer *ren, const std::string &path, const std::string &ext) {
    std::vector<SDL_Texture*> vec;
    SDL_Texture *tex;
    int number = 0;
    while ((tex = IMG_LoadTexture(ren, (path+"/"+std::to_string(number++)+"."+ext).c_str())) != nullptr)
        vec.push_back(tex);
    return vec;
}
void renderRect(SDL_Renderer *ren, int x, int y, int w, int h, SDL_Color col) {
    SDL_Rect rectangle = {x, y, w, h};
    SDL_SetRenderDrawColor(ren, col.r, col.g, col.b, col.a);
    SDL_RenderFillRect(ren, &rectangle);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
}
void renderText(SDL_Renderer *ren, int x, int y, int w, int h, const std::string &text, TTF_Font *font, SDL_Color col) {
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text.c_str(), col);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(ren, textSurface);
    renderTexture(ren, textTexture, x, y, w, h, SDL_FLIP_NONE);
}
