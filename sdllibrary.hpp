//
//  sdllibrary.hpp
//  RPG
//
//  Created by Иван Гаврилов on 05.11.2019.
//  Copyright © 2019 Иван Гаврилов. All rights reserved.
//

#ifndef sdllibrary_hpp
#define sdllibrary_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "GameClasses.hpp"
SDL_Texture* LoadImage(SDL_Renderer *ren, const std::string &path);
void renderTexture(SDL_Renderer *ren, SDL_Texture *tex, int x, int y, int w, int h, SDL_RendererFlip flip);
void renderTexture(SDL_Renderer *ren, SDL_Texture *tex, int x, int y, SDL_RendererFlip flip);
std::vector<SDL_Texture*> LoadImages(SDL_Renderer *ren, const std::string &path, const std::string &ext);
void renderRect(SDL_Renderer *ren, int x, int y, int w, int h, SDL_Color col);
void renderText(SDL_Renderer *ren, int x, int y, int w, int h, const std::string &text, TTF_Font *font, SDL_Color col);
#endif /* sdllibrary_hpp */
