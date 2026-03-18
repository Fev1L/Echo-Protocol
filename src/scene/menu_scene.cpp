//
// Created by Fev1L on 18.03.2026.
//

#include "scene.h"

void renderMenu(Game* game, App* app) {
    float mouseX, mouseY;
    SDL_GetMouseState(&mouseX,&mouseY);

    if (SDL_GetAudioStreamQueued(app->audio->menuBackgroundSong.stream) < (int)app->audio->menuBackgroundSong.Len)
        SDL_PutAudioStreamData(app->audio->menuBackgroundSong.stream, app->audio->menuBackgroundSong.Data, app->audio->menuBackgroundSong.Len);

    float t = SDL_GetTicks() * 0.002f;
    float flicker = 0.9f + sin(t * 5.0f) * 0.04f + sin(t * 13.0f) * 0.02f;
    Uint8 lampAlpha = (Uint8)(flicker * 50);
    SDL_RenderTexture(app->renderer, game->menu.menuBackground, NULL, NULL);

    SDL_FRect logoRect = {layout(Anchor::TOP_LEFT, 0.256f, 0.173f, 0.034f, 0.101f, app->state->winW, app->state->winH)};
    drawImage(app->renderer, game->menu.menuLogo, logoRect, app , ViewSide::CENTER);

    if(isTextClicked(app->renderer, app->fonts->font2, game->menu.newGame, app, mouseX, mouseY)){
        game->menu.newGame.textIn = "> NEW GAME";
        buildText(app->renderer, app->fonts->font2, game->menu.newGame);
    }else{
        if(game->menu.newGame.textIn != "NEW GAME"){
            game->menu.newGame.textIn = "NEW GAME";
            buildText(app->renderer, app->fonts->font2, game->menu.newGame);
        }
    }

    if(isTextClicked(app->renderer, app->fonts->font2, game->menu.continueGame, app, mouseX, mouseY)){
        game->menu.continueGame.textIn = "> CONTINUE GAME";
        buildText(app->renderer, app->fonts->font2, game->menu.continueGame);
    }else{
        if(game->menu.newGame.textIn != "CONTINUE GAME"){
            game->menu.continueGame.textIn = "CONTINUE GAME";
            buildText(app->renderer, app->fonts->font2, game->menu.continueGame);
        }
    }

    if(isTextClicked(app->renderer, app->fonts->font2, game->menu.customGame, app, mouseX, mouseY)){
        game->menu.customGame.textIn = "> CUSTOM GAME";
        buildText(app->renderer, app->fonts->font2, game->menu.customGame);
    }else{
        if(game->menu.newGame.textIn != "CUSTOM GAME"){
            game->menu.customGame.textIn = "CUSTOM GAME";
            buildText(app->renderer, app->fonts->font2, game->menu.customGame);
        }
    }

    drawText(app->renderer, game->menu.newGame, app);
    drawText(app->renderer, game->menu.continueGame, app);
    drawText(app->renderer, game->menu.continueGameNight, app);
    if(game->currentNight >= 8) drawText(app->renderer, game->menu.customGame, app);

    SDL_FRect baseLampRect = {layout(Anchor::TOP_LEFT, 0.277f, 0.390f, 0.65f, 0.199f, app->state->winW, app->state->winH)};
    SDL_FRect lampRect;
    float scale = 1.0f + sin(t * 3.0f) * 0.01f;
    lampRect.w = baseLampRect.w * scale;
    lampRect.h = baseLampRect.h * scale;
    lampRect.x = baseLampRect.x - (lampRect.w - baseLampRect.w) / 2.0f;
    lampRect.y = baseLampRect.y - (lampRect.h - baseLampRect.h) / 2.0f;

    SDL_SetTextureAlphaMod(game->menu.lampGlowTexture, lampAlpha);
    SDL_RenderTexture(app->renderer, game->menu.lampGlowTexture, NULL, &lampRect);

    SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, (1.0f - game->menu.menuFade) * 255);
    SDL_RenderFillRect(app->renderer, NULL);
}
