//
//  scene.cpp
//  Echo Protocol
//
//  Created by Fev1L on 16.02.2026.
//

#include "scene.h"

void renderCustomgame(Game* game, App* app) {
    
}

void renderEndgame(Game* game, App* app) {
    SDL_SetRenderDrawColor(app->renderer, 2, 2, 2, 255);
    app->fonts->endGameText.textIn = "NIGHT " + std::to_string(game->currentNight);
    buildText(app->renderer, app->fonts->font1, app->fonts->endGameText);

    drawText(app->renderer, app->fonts->endGameText, app);
}

void renderWinScreen(Game* game, App* app) {
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);

    drawText(app->renderer, app->fonts->winTitle, app);
    drawText(app->renderer, app->fonts->winSubtitle, app);
}
