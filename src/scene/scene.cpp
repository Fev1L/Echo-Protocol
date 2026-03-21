//
//  scene.cpp
//  Echo Protocol
//
//  Created by Fev1L on 16.02.2026.
//

#include "scene.h"

void renderCustomgame(Game* game, App* app) {
    SDL_SetRenderDrawColor(app->renderer, 8, 8, 8, 255);
    SDL_RenderFillRect(app->renderer, NULL);

    SDL_Color normal = {255,255,255,255};
    SDL_Color selected = {255,255,0,255};

    drawText(app->renderer, game->customGame.customTitle, app);

    drawText(app->renderer,  game->customGame.customMonsterCount, app);
    drawText(app->renderer,  game->customGame.customMoveInterval, app);
    drawText(app->renderer,  game->customGame.customEchoInterval, app);
    drawText(app->renderer,  game->customGame.customBreakChance, app);
    drawText(app->renderer,  game->customGame.customBaitReload, app);
    drawText(app->renderer,  game->customGame.customTimeSpeed, app);

    drawText(app->renderer, game->customGame.customStart, app);
    drawText(app->renderer, game->customGame.customBack, app);
}

void renderEndgame(Game* game, App* app) {
    SDL_SetRenderDrawColor(app->renderer, 2, 2, 2, 255);

    drawText(app->renderer, app->fonts->endGameText, app);
}

void renderWinScreen(Game* game, App* app) {
    SDL_SetRenderDrawColor(app->renderer, 2, 2, 2, 255);

    drawText(app->renderer, app->fonts->winTitle, app);
    drawText(app->renderer, app->fonts->winSubtitle, app);
}

void renderLoseScreen(Game* game, App* app) {
    State* state = app->state;
    Font* fonts = app->fonts;

    float t = game->loseTimer;

    SDL_FRect fullScreen = {0, 0, (float)state->winW, (float)state->winH};

    SDL_SetRenderDrawColor(app->renderer, 2, 2, 2, 255);
    SDL_RenderFillRect(app->renderer, &fullScreen);

    if (t < 1.35f) {
        float alpha = (t / 0.35f) * 255.0f;
        SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, (Uint8)alpha);
        SDL_RenderFillRect(app->renderer, &fullScreen);
        return;
    }

    if (t >= 1.35f && t < 10.0f) {
        updateCamera(game, app->deltaTime);
        SDL_FRect gameBackgroundRect = {0, 0, (float)state->winW, (float)state->winH};

        drawImage(app->renderer, state->gameBackgroundTexture, gameBackgroundRect, app, ViewSide::CENTER);
        drawImage(app->renderer, state->gameBackgroundTextureRight, gameBackgroundRect, app, ViewSide::RIGHT);
        drawImage(app->renderer, state->gameBackgroundTextureLeftOpen, gameBackgroundRect, app, ViewSide::LEFT);

        SDL_FRect topLightRect = {
            app->state->winW * 0.18f,
            -app->state->winH * 0.08f,
            app->state->winW * 0.64f,
            app->state->winH * 0.42f
        };

        float t = SDL_GetTicks() * 0.001f;

        SDL_Texture* lampTexture = app->state->topRedTexture;
        Uint8 lampAlpha = (Uint8)(game->topLamp.intensity * 115.0f);
        float pulse = (sin(t * 12.0f) > 0.0f) ? 1.0f : 0.35f;
        lampAlpha = (Uint8)(50 + pulse * 150);

        SDL_SetTextureAlphaMod(lampTexture, lampAlpha);
        SDL_RenderTexture(app->renderer, lampTexture, NULL, &topLightRect);

        Uint8 darkness = 120;
        SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, darkness);
        SDL_RenderFillRect(app->renderer, &fullScreen);

        return;
    }

    SDL_SetRenderDrawColor(app->renderer, 2, 2, 2, 255);
    SDL_RenderFillRect(app->renderer, &fullScreen);

    if (t >= 12.5f) {
        game->currentView == ViewSide::CENTER;
        game->viewAngle = 0;
        drawText(app->renderer, fonts->loseText, app);
        game->loseCanExit = true;
    }
}

void renderTutorial(Game* game, App* app) {
    SDL_SetRenderDrawColor(app->renderer, 5, 5, 5, 255);
    SDL_RenderFillRect(app->renderer, NULL);

    Text tutorialText;
    tutorialText.rect = layoutText(0.35f, 0.46f, app->state->winW, app->state->winH);
    tutorialText.color = {255, 255, 255, 255};
    tutorialText.label = "tutorial";
    tutorialText.side = ViewSide::CENTER;

    switch (game->tutorialStep) {
    case 0:
        tutorialText.textIn = "USE A AND D TO SWITCH BETWEEN SCREENS";
        buildText(app->renderer, app->fonts->font1, tutorialText);
        break;
    case 1:
        tutorialText.textIn = "CLICK THE GRID TO DISTRACT THE MONSTER";
        buildText(app->renderer, app->fonts->font1, tutorialText);
        break;
    case 2:
        tutorialText.textIn = "REPAIR SYSTEMS BEFORE IT IS TOO LATE";
        buildText(app->renderer, app->fonts->font1, tutorialText);
        break;
    default:
        tutorialText.textIn = "";
        buildText(app->renderer, app->fonts->font1, tutorialText);
        break;
    }

    drawText(app->renderer, tutorialText, app);
}