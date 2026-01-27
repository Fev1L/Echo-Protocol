//
//  render.cpp
//  Echo Protocol
//
//  Created by Fev1L on 11.01.2026.
//

#include "render.h"

void drawRectangle(SDL_Renderer* renderer, const Rectangle& rct, App* app) {
    SDL_SetRenderDrawColor(renderer,
        rct.color.r, rct.color.g, rct.color.b, rct.color.a);

    SDL_FRect r = rct.rect;

    float cx = app->state->winW * 0.5f;
    float cy = app->state->winH * 0.5f;

    float t = fabs(app->game->viewAngle) / 90.0f;
    t = SDL_clamp(t, 0.0f, 1.0f);

    float offsetX = (app->game->viewAngle / 90.0f) * app->state->winW * 0.5f;
    float scale   = 1.0f - 0.25f * t;
    
    float sideX = sideOffsetX(rct.side, app->state->winW);
    
    r.x += sideX;

    r.x = (r.x - cx) * scale + cx + offsetX;
    r.y = (r.y - cy) * scale + cy;

    r.w *= scale;
    r.h *= scale;

    SDL_RenderFillRect(renderer, &r);
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
//=================================================================
SDL_FRect layoutText(float marginXPct, float marginYPct, int winW, int winH){
    float x = 0, y = 0;
    x = winW * marginXPct;
    y = winH * marginYPct;
    return {x, y};
}
//=================================================================
float sideOffsetX(ViewSide side, float screenW) {
    switch (side) {
        case ViewSide::LEFT:   return -screenW;
        case ViewSide::RIGHT:  return +screenW;
        default:               return 0.0f;
    }
}
