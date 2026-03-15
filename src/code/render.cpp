//
//  render.cpp
//  Echo Protocol
//
//  Created by Fev1L on 11.01.2026.
//

#include "render.h"

SDL_FRect getTextScreenRect(SDL_Renderer* renderer, TTF_Font* font, const Text& text, const App* app) {
    int textW = 0;
    int textH = 0;

    TTF_GetStringSize(font, text.textIn.c_str(), text.textIn.size(), &textW, &textH);

    float scale = app->state->winW / 1920.0f;

    float sideX = sideOffsetX(text.side, app->state->winW);
    float camOffset = -(app->game->viewAngle / 90.0f) * app->state->winW;

    SDL_FRect r;

    r.w = textW * scale;
    r.h = textH * scale;

    r.x = text.rect.x + sideX + camOffset;
    r.y = text.rect.y;

    return r;
}
//=================================================================
void drawRectangle(SDL_Renderer* renderer, const Rectangle& rct, App* app) {
    SDL_SetRenderDrawColor(renderer, rct.color.r, rct.color.g, rct.color.b, rct.color.a);
    SDL_FRect r = rct.rect;

    float sideX = sideOffsetX(rct.side, app->state->winW);
    float camOffset = -(app->game->viewAngle / 90.0f) * app->state->winW;

    r.x += sideX + camOffset;
    SDL_RenderFillRect(renderer, &r);
}
//=================================================================
bool isButtonClicked(SDL_FRect rect, float x, float y) {
    return (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h);
}
//=================================================================
bool isTextClicked(SDL_Renderer* renderer, TTF_Font* font, const Text& text, const App* app, float x, float y){
    SDL_FRect r = getTextScreenRect(renderer, font, text, app);
    return (x >= r.x && x <= r.x + r.w && y >= r.y && y <= r.y + r.h);
}
//=================================================================
void drawText(SDL_Renderer* renderer, TTF_Font* font, const Text& text, const App* app){
    SDL_Color color = {text.color.r, text.color.g, text.color.b, 255};

    SDL_Surface* surface = TTF_RenderText_Blended(font, text.textIn.c_str(), 0, color);
    if (!surface) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    if (!texture) return;

    SDL_FRect dst = getTextScreenRect(renderer, font, text, app);

    SDL_RenderTexture(renderer, texture, nullptr, &dst);
    SDL_DestroyTexture(texture);
}
//=================================================================
void drawImage(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_FRect& targetRect, App* app, ViewSide side){
    if (!texture) return;
    SDL_FRect r = targetRect;

    float sideX = sideOffsetX(side, app->state->winW);
    float camOffset = -(app->game->viewAngle / 90.0f) * app->state->winW;

    r.x += sideX + camOffset;
    SDL_RenderTexture(renderer, texture, nullptr, &r);
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
