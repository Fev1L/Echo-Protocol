//
//  scene.cpp
//  Echo Protocol
//
//  Created by Fev1L on 16.02.2026.
//

#include "scene.h"

void renderMenu(Game* game, App* app) {
    game->menu.newGame = {{layoutText(0.034f, 0.477f, app->state->winW, app->state->winH)}, {255,255,255,255},"newGame", "NEW GAME", ViewSide::CENTER};
    game->menu.continueGame = {{layoutText(0.034f, 0.536f, app->state->winW, app->state->winH)}, {255,255,255,255},"newGame", "CONTINUE GAME", ViewSide::CENTER};
    game->menu.settings = {{layoutText(0.034f, 0.594f, app->state->winW, app->state->winH)}, {255,255,255,255},"newGame", "SETTINGS", ViewSide::CENTER};

    drawText(game->renderer, app->fonts->font1, game->menu.newGame, app);
    drawText(game->renderer, app->fonts->font1, game->menu.continueGame, app);
    drawText(game->renderer, app->fonts->font1, game->menu.settings, app);
}

void updateMenu(Game* game, App* app, SDL_Event& event){
    float mouseX = event.button.x;
    float mouseY = event.button.y;
    
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN){
        if (isTextClicked(game->renderer, app->fonts->font1, game->menu.newGame, app, mouseX, mouseY)) {
            startNewGame(game);
            app->gamestate = GameState::PLAYING;
        }
        if (isTextClicked(game->renderer, app->fonts->font1, game->menu.continueGame, app, mouseX, mouseY)) {
            loadGame(game);
            app->gamestate = GameState::PLAYING;
        }
        if (isTextClicked(game->renderer, app->fonts->font1, game->menu.settings, app, mouseX, mouseY))
            app->gamestate = GameState::SETTINGS;
    }
}
