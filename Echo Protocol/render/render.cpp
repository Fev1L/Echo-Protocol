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

bool isButtonClicked(SDL_FRect rect, float x, float y) {
    return (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h);
}

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

void drawImage(SDL_Renderer* renderer, SDL_Texture* imageTexture, const Image& img){
    SDL_FRect imageRect = {img.rect.x, img.rect.y, img.rect.h, img.rect.w};
    SDL_RenderTexture(renderer, imageTexture, nullptr, &imageRect);
}

void drawTriangle(SDL_Renderer* r, const Triangle& t){
    SDL_SetRenderDrawColor(r, t.color.r, t.color.g, t.color.b, t.color.a);

    SDL_RenderLine(r, t.p1.x, t.p1.y, t.p2.x, t.p2.y);
    SDL_RenderLine(r, t.p2.x, t.p2.y, t.p3.x, t.p3.y);
    SDL_RenderLine(r, t.p3.x, t.p3.y, t.p1.x, t.p1.y);
}

void fillTriangle(SDL_Renderer* r, Triangle t){
    SDL_SetRenderDrawColor(r, t.color.r, t.color.g, t.color.b, t.color.a);

    if (t.p2.y < t.p1.y) std::swap(t.p1, t.p2);
    if (t.p3.y < t.p1.y) std::swap(t.p1, t.p3);
    if (t.p3.y < t.p2.y) std::swap(t.p2, t.p3);

    auto interp = [&](SDL_FPoint a, SDL_FPoint b){
        std::vector<float> vals;
        int dy = int(b.y - a.y);
        if (dy < 1) return vals;
        vals.reserve(dy);
        for (int i = 0; i <= dy; i++) {
            float tVal = float(i) / dy;
            vals.push_back(a.x + (b.x - a.x) * tVal);
        }
        return vals;
    };

    auto x12 = interp(t.p1, t.p2);
    auto x23 = interp(t.p2, t.p3);
    auto x13 = interp(t.p1, t.p3);

    x12.insert(x12.end(), x23.begin(), x23.end());

    int total = int(x13.size());
    for (int i = 0; i < total; i++) {
        float xL = x12[i];
        float xR = x13[i];
        if (xL > xR) std::swap(xL, xR);
        float y = t.p1.y + i;
        SDL_RenderLine(r, xL, y, xR, y);
    }
}

void drawBevel(SDL_Renderer* renderer, Bavel bvl, bool pressed)
{
    SDL_Color light = {255, 255, 255, 255};
    SDL_Color dark  = {129, 129, 129, 255};

    SDL_SetRenderDrawColor(renderer, 191, 191, 191, 255);
    SDL_RenderFillRect(renderer, &bvl.rect);

    SDL_Color topLeft  = pressed ? light : dark;
    SDL_Color bottomRight = pressed ? dark : light;

    Rectangle rectangleSides[] = {
        {{bvl.rect.x, bvl.rect.y, 4, bvl.rect.h}, topLeft, 0.0f, "left"},
        {{bvl.rect.x, bvl.rect.y, bvl.rect.w, 4}, topLeft, 0.0f, "top"},
        {{bvl.rect.x, bvl.rect.y + bvl.rect.h - 4, bvl.rect.w, 4}, bottomRight, 0.0f, "bottom"},
        {{bvl.rect.x + bvl.rect.w - 4, bvl.rect.y, 4, bvl.rect.h}, bottomRight, 0.0f, "right"}
    };

    Triangle tri[] = {
        {
            {bvl.rect.x, bvl.rect.y + bvl.rect.h - 4},
            {bvl.rect.x + 4, bvl.rect.y + bvl.rect.h - 4},
            {bvl.rect.x, bvl.rect.y + bvl.rect.h},
            bottomRight
        },
        {
            {bvl.rect.x + bvl.rect.w - 4, bvl.rect.y},
            {bvl.rect.x + bvl.rect.w, bvl.rect.y},
            {bvl.rect.x + bvl.rect.w - 4, bvl.rect.y + 4},
            topLeft
        }
    };

    for (Rectangle rct : rectangleSides)
        drawRectangle(renderer, rct);

    for (Triangle t : tri) {
        drawTriangle(renderer, t);
        fillTriangle(renderer, t);
    }
}

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

