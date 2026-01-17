//
//  render.cpp
//  Echo Protocol
//
//  Created by Fev1L on 11.01.2026.
//

#include "render.h"

void drawRectangle(SDL_Renderer* renderer,const Rectangle &rct) {
    SDL_SetRenderDrawColor(renderer, rct.color.r, rct.color.g, rct.color.b, rct.color.a);

    SDL_FRect core = { rct.rect.x + rct.radius, rct.rect.y, rct.rect.w - 2 * rct.radius, rct.rect.h };
    SDL_RenderFillRect(renderer, &core);

    SDL_FRect left = { rct.rect.x, rct.rect.y + rct.radius, rct.radius, rct.rect.h - 2 * rct.radius };
    SDL_FRect right = { rct.rect.x + rct.rect.w - rct.radius, rct.rect.y + rct.radius, rct.radius, rct.rect.h - 2 * rct.radius };
    SDL_RenderFillRect(renderer, &left);
    SDL_RenderFillRect(renderer, &right);

    for (float w = 0; w < rct.radius; ++w) {
        for (float h = 0; h < rct.radius; ++h) {
            if ((w * w + h * h) <= rct.radius * rct.radius) {
                SDL_RenderPoint(renderer, rct.rect.x + rct.radius - w, rct.rect.y + rct.radius - h);
                SDL_RenderPoint(renderer, rct.rect.x + rct.rect.w - rct.radius + w - 1, rct.rect.y + rct.radius - h);
                SDL_RenderPoint(renderer, rct.rect.x + rct.radius - w, rct.rect.y + rct.rect.h - rct.radius + h - 1);
                SDL_RenderPoint(renderer, rct.rect.x + rct.rect.w - rct.radius + w - 1, rct.rect.y + rct.rect.h - rct.radius + h - 1);
            }
        }
    }
}
//=================================================================
bool isButtonClicked(SDL_FRect rect, float x, float y) {
    return (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h);
}
//=================================================================
void drawText(SDL_Renderer* renderer,TTF_Font* font, const Text& text){
    float textW, textH;
    SDL_Color color = {text.color.r, text.color.g, text.color.b, 255};
    SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, text.textIn.c_str(), 0, color, 375);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    SDL_GetTextureSize(texture, &textW, &textH);
    SDL_FRect dst = { text.rect.x, text.rect.y, textW, textH };
    SDL_RenderTexture(renderer, texture, nullptr, &dst);
}
//=================================================================
void drawImage(SDL_Renderer* renderer, SDL_Texture* imageTexture, const Image& img){
    SDL_FRect imageRect = {img.rect.x, img.rect.y, img.rect.h, img.rect.w};
    SDL_RenderTexture(renderer, imageTexture, nullptr, &imageRect);
}
//=================================================================
SDL_FRect layout( Anchor anchor, float wPct, float hPct, float marginXPct, float marginYPct, int winW, int winH){
    float w = winW * wPct;
    float h = winH * hPct;

    float x = 0, y = 0;

    switch (anchor) {
        case Anchor::TOP_LEFT:
            x = winW * marginXPct;
            y = winH * marginYPct;
            break;

        case Anchor::TOP_RIGHT:
            x = winW * (1.0f - wPct - marginXPct);
            y = winH * marginYPct;
            break;

        case Anchor::BOTTOM_LEFT:
            x = winW * marginXPct;
            y = winH * (1.0f - hPct - marginYPct);
            break;

        case Anchor::BOTTOM_RIGHT:
            x = winW * (1.0f - wPct - marginXPct);
            y = winH * (1.0f - hPct - marginYPct);
            break;

        case Anchor::CENTER:
            x = winW * (0.5f - wPct * 0.5f);
            y = winH * (0.5f - hPct * 0.5f);
            break;
    }

    return {x, y, w, h};
}

