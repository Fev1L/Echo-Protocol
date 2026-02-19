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
    game->menu.customGame = {{layoutText(0.034f, 0.594f, app->state->winW, app->state->winH)}, {255,255,255,255},"newGame", "SETTINGS", ViewSide::CENTER};

    drawText(app->renderer, app->fonts->font1, game->menu.newGame, app);
    drawText(app->renderer, app->fonts->font1, game->menu.continueGame, app);
    drawText(app->renderer, app->fonts->font1, game->menu.customGame, app);
}

void renderGame(Game* game, App* app) {
    State* state = app->state;
    Font* fonts = app->fonts;
    
    state->table = {{layout(
        Anchor::TOP_LEFT,
        1.0f, 0.20f, //SIZE
        0.00f, 0.70f, //MARGIN
        app->state->winW,app->state->winH)},{143,103,33,255},"TABLE",ViewSide::CENTER};
    state->monitor = {{layout(
        Anchor::TOP_LEFT,
        0.555f, 0.585f, //SIZE
        0.222f, 0.289f, //MARGIN
        state->winW,state->winH)},{0,0,0,255},"Computer", ViewSide::CENTER};
    for(int i = 0; i < game->GRID_H; i++){
        for(int j = 0; j < game->GRID_W; j++){
            state->rooms.push_back(
               {{
                   layout(Anchor::TOP_LEFT,
                          0.0069f, 0.0097f,
                          (0.239f + (0.0069f * j)),
                          (0.313f + (0.0097f * i)),
                          state->winW,
                          state->winH
                          )
               },
                   {120,120,120,255},
                   "WEIGHT",
                   ViewSide::CENTER
               }
            );
        }
    }
    fonts->night = {{layoutText(0.006f, 0.009f, state->winW, state->winH)}, {255,255,255,255},"Night", "NIGHT " + std::to_string(game->currentNight), ViewSide::CENTER};
    
    state->tableR = {{layout(
        Anchor::TOP_LEFT,
        1.00f, 0.20f, //SIZE
        0.00f, 0.70f, //MARGIN
        app->state->winW,app->state->winH)},{143,103,33,255},"TABLE",ViewSide::RIGHT};
    state->monitorR = {{layout(
        Anchor::TOP_LEFT,
        0.555f, 0.585f, //SIZE
        0.222f, 0.289f, //MARGIN
        state->winW,state->winH)},{0,0,0,255},"Computer", ViewSide::RIGHT};

    if(game->system.baitSystem){
        fonts->baitSystem = {{layoutText(0.246f, 0.323f, state->winW, state->winH)}, {0,255,0,255},"baitSystem", "BAIT SYSTEM", ViewSide::RIGHT};
    }else{
        fonts->baitSystem = {{layoutText(0.246f, 0.323f, state->winW, state->winH)}, {255,0,0,255},"baitSystem", "BAIT SYSTEM", ViewSide::RIGHT};
    }
    if(game->system.echoSystem){
        fonts->echoSystem = {{layoutText(0.246f, 0.373f, state->winW, state->winH)}, {0,255,0,255},"echoSystem", "ECHO SYSTEM", ViewSide::RIGHT};
    }else{
        fonts->echoSystem = {{layoutText(0.246f, 0.373f, state->winW, state->winH)}, {255,0,0,255},"echoSystem", "ECHO SYSTEM", ViewSide::RIGHT};
    }
    if(game->system.trackingSystem){
        fonts->trackingSystem = {{layoutText(0.246f, 0.422f, state->winW, state->winH)}, {0,255,0,255},"trackingSystem", "TRACKING SYSTEM", ViewSide::RIGHT};
    }else{
        fonts->trackingSystem = {{layoutText(0.246f, 0.422f, state->winW, state->winH)}, {255,0,0,255},"trackingSystem", "TRACKING SYSTEM", ViewSide::RIGHT};
    }
    fonts->rebootAll = {{layoutText(0.246f, 0.5f, state->winW, state->winH)}, {0,255,0,255},"rebootAll", "REBOOT ALL", ViewSide::RIGHT};
    
    for(int i = 0; i < game->GRID_H; i++){
        for(int j = 0; j < game->GRID_W; j++){

            float baseR = 11.0f;
            float baseG = 47.0f;
            float baseB = 0.0f;

            float lightR = 0.0f;
            float lightG = 0.0f;
            float lightB = 0.0f;

            for(const auto& noise : game->noise){
                if (!noise.active) continue;

                int dx = j - noise.x;
                int dy = i - noise.y;
                float dist = sqrtf(dx * dx + dy * dy);

                if (dist <= noise.radius){
                    float t = 1.0f - (dist / noise.radius);

                    t = t * t;

                    lightR += 160.0f * t;
                    lightG += 170.0f * t;
                    lightB += 120.0f * t;
                }
            }

            if (game->echo.active){
                int dx = j - game->centerX;
                int dy = i - game->centerY;
                float dist = sqrtf(dx*dx + dy*dy);

                float diff = fabs(dist - game->echo.radius);

                if (diff < 1.0f){
                    float t = 1.0f - diff;
                    lightR += 255.0f * t;
                    lightG += 255.0f * t;
                    lightB += 255.0f * t;
                }
            }

            float finalR = baseR + lightR;
            float finalG = baseG + lightG;
            float finalB = baseB + lightB;

            finalR = SDL_clamp(finalR, 0.0f, 255.0f);
            finalG = SDL_clamp(finalG, 0.0f, 255.0f);
            finalB = SDL_clamp(finalB, 0.0f, 255.0f);

            SDL_Color color = {
                (Uint8)finalR,
                (Uint8)finalG,
                (Uint8)finalB,
                255
            };

            for(const auto& monster : game->monsters){
                if (monster.present && monster.visible &&
                    i == monster.echoY && j == monster.echoX)
                {
                    color = {255, 60, 60, 255};
                }
            }

            if (i == game->centerY && j == game->centerX)
                color = {0, 255, 0, 255};

            state->rooms.push_back({
                {layout(Anchor::TOP_LEFT, 0.0069f, 0.0097f,
                        (0.239f + (0.0069f * j)),
                        (0.313f + (0.0097f * i)),
                        state->winW, state->winH)},
                color,
                "WEIGHT",
                ViewSide::CENTER
            });
        }
    }
    
    drawRectangle(app->renderer, state->table, app);
    drawRectangle(app->renderer, state->tableR, app);
    drawRectangle(app->renderer, state->monitor, app);
    drawRectangle(app->renderer, state->monitorR, app);
    for(Rectangle rec : state->rooms){
        drawRectangle(app->renderer, rec, app);
    }
    drawText(app->renderer, fonts->font1, fonts->night, app);
    
    std::ostringstream ss;
    ss << std::setw(2) << std::setfill('0') << game->hours
       << ":"
       << std::setw(2) << std::setfill('0') << game->minutes;

    fonts->hours = {{layoutText(0.006f, 0.029f, state->winW, state->winH)}, {255,255,255,255},"Hours", ss.str(), ViewSide::CENTER};
    drawText(app->renderer, fonts->font1, fonts->hours, app);

    drawText(app->renderer, fonts->font1, fonts->bait, app);
    drawText(app->renderer, fonts->font1, fonts->baitSystem, app);
    drawText(app->renderer, fonts->font1, fonts->echoSystem, app);
    drawText(app->renderer, fonts->font1, fonts->trackingSystem, app);
    drawText(app->renderer, fonts->font1, fonts->rebootAll, app);
}

void renderCustomgame(Game* game, App* app) {
    
}

void renderEndgame(Game* game, App* app) {
    app->fonts->endGameText = {{layoutText(0.45f, 0.5f, app->state->winW, app->state->winH)}, {255,255,255,255},"endGame", "NIGHT " + std::to_string(game->currentNight), ViewSide::CENTER};
        
    drawText(app->renderer, app->fonts->font1, app->fonts->endGameText, app);
}
