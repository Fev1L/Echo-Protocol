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
void buildText(SDL_Renderer* renderer, TTF_Font* font, Text& text){
    if(text.texture) SDL_DestroyTexture(text.texture);
    SDL_Color color = {text.color.r, text.color.g, text.color.b, 255};

    SDL_Surface* surface = TTF_RenderText_Blended(
        font,
        text.textIn.c_str(),
        0,
        color
    );

    if (!surface) return;

    text.texture = SDL_CreateTextureFromSurface(renderer, surface);

    text.w = surface->w;
    text.h = surface->h;

    SDL_DestroySurface(surface);
}
//=================================================================
void drawText(SDL_Renderer* renderer, const Text& text, const App* app){
    if (!text.texture) return;

    float scale = app->state->winW / 1920.0f;

    float sideX = sideOffsetX(text.side, app->state->winW);
    float camOffset = -(app->game->viewAngle / 90.0f) * app->state->winW;

    SDL_FRect r;

    r.w = text.w * scale;
    r.h = text.h * scale;

    r.x = text.rect.x + sideX + camOffset;
    r.y = text.rect.y;

    SDL_RenderTexture(renderer, text.texture, nullptr, &r);
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
//=================================================================
void buildGrid(App* app){
    Game* game = app->game;
    State* state = app->state;

    state->rooms.clear();
    state->rooms.reserve(game->GRID_W * game->GRID_H);

    for(int i = 0; i < game->GRID_H; i++)
    {
        for(int j = 0; j < game->GRID_W; j++)
        {
            Rectangle r;

            r.rect = layout(
                Anchor::TOP_LEFT,
                0.0052f, 0.0092f,
                0.184f + (0.0052f * j),
                0.212f + (0.0092f * i),
                state->winW,
                state->winH
            );

            r.color = {11,47,0,255};
            r.label = "CELL";
            r.side = ViewSide::CENTER;

            state->rooms.push_back(r);
        }
    }
}