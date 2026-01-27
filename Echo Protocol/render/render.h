//
//  render.h
//  Echo Protocol
//
//  Created by Fev1L on 11.01.2026.
//

#pragma once
#include "../types.h"

void drawRectangle(SDL_Renderer* renderer,const Rectangle &rct, App* app);

bool isButtonClicked(SDL_FRect rect, float x, float y);

void drawText(SDL_Renderer* renderer,TTF_Font* font, const Text& text);

void drawImage(SDL_Renderer* renderer, SDL_Texture* imageTexture, const Image& img);

SDL_FRect layout(Anchor anchor, float wPct, float hPct, float marginXPct, float marginYPct, int winW, int winH);

SDL_FRect layoutText(float marginXPct, float marginYPct, int winW, int winH);

float sideOffsetX(ViewSide side, float screenW);
