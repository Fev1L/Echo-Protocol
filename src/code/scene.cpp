//
//  scene.cpp
//  Echo Protocol
//
//  Created by Fev1L on 16.02.2026.
//

#include "scene.h"

void renderMenu(Game* game, App* app) {
    float mouseX, mouseY;
    SDL_GetMouseState(&mouseX,&mouseY);
    
    if (SDL_GetAudioStreamQueued(app->audio->menuBackgroundSong.stream) < (int)app->audio->menuBackgroundSong.Len) SDL_PutAudioStreamData(app->audio->menuBackgroundSong.stream, app->audio->menuBackgroundSong.Data, app->audio->menuBackgroundSong.Len);
    

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

void renderGame(Game* game, App* app) {
    State* state = app->state;
    Font* fonts = app->fonts;
    
    SDL_FRect gameBackgroundRect = {0, 0, static_cast<float>(app->state->winW), static_cast<float>(app->state->winH)};
    drawImage(app->renderer, app->state->gameBackgroundTexture, gameBackgroundRect,app, ViewSide::CENTER);
    drawImage(app->renderer, app->state->gameBackgroundTextureRight, gameBackgroundRect,app, ViewSide::RIGHT);
    drawImage(app->renderer, app->state->gameBackgroundTextureLeftClose, gameBackgroundRect,app, ViewSide::LEFT);

    constexpr SDL_Color red = {255, 0, 0, 255};
    constexpr SDL_Color green = {0, 255, 0, 255};

    if (game->system.baitSystem){
        if (fonts->baitSystem.color.g != green.g){
            fonts->baitSystem.color = green;
            buildText(app->renderer, fonts->font1, fonts->baitSystem);
        }
    }else{
        if (fonts->baitSystem.color.r != red.r){
            fonts->baitSystem.color = red;
            buildText(app->renderer, fonts->font1, fonts->baitSystem);
        }
    }

    if (game->system.echoSystem){
        if (fonts->echoSystem.color.g != green.g){
            fonts->echoSystem.color = green;
            buildText(app->renderer, fonts->font1, fonts->echoSystem);
        }
    }else{
        if (fonts->echoSystem.color.r != red.r){
            fonts->echoSystem.color = red;
            buildText(app->renderer, fonts->font1, fonts->echoSystem);
        }
    }

    if (game->system.trackingSystem){
        if (fonts->trackingSystem.color.g != green.g){
            fonts->trackingSystem.color = green;
            buildText(app->renderer, fonts->font1, fonts->trackingSystem);
        }
    }else{
        if (fonts->trackingSystem.color.r != red.r){
            fonts->trackingSystem.color = red;
            buildText(app->renderer, fonts->font1, fonts->trackingSystem);
        }
    }

    for(int i = 0; i < game->GRID_H; i++) {
        for(int j = 0; j < game->GRID_W; j++){
            int idx = i * game->GRID_W + j;
            Rectangle& cell = state->rooms[idx];

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

                float dist2 = dx*dx + dy*dy;
                float r2 = noise.radius * noise.radius;

                if(dist2 <= r2)
                {
                    float dist = sqrtf(dist2);
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

            float finalR = SDL_clamp(baseR + lightR, 0.0f, 255.0f);
            float finalG = SDL_clamp(baseG + lightG, 0.0f, 255.0f);
            float finalB = SDL_clamp(baseB + lightB, 0.0f, 255.0f);

            SDL_Color color = {
                (Uint8)finalR,
                (Uint8)finalG,
                (Uint8)finalB,
                255
            };

            for(const auto& monster : game->monsters){
                if(monster.present && monster.visible &&
                   i == monster.echoY && j == monster.echoX)
                {
                    color = {255,60,60,255};
                }
            }

            if(i == game->centerY && j == game->centerX)
                color = {0,255,0,255};

            cell.color = color;
        }
    }

    for(const Rectangle rec : state->rooms){
        drawRectangle(app->renderer, rec, app);
    }
    drawText(app->renderer, fonts->night, app);

    std::ostringstream ss;
    ss << std::setw(2) << std::setfill('0') << game->hours
       << ":"
       << std::setw(2) << std::setfill('0') << game->minutes;


    if (fonts->hours.textIn != ss.str()){
        fonts->hours.textIn = ss.str();
        buildText(app->renderer, fonts->font1, fonts->hours);
    }

    drawText(app->renderer, fonts->hours, app);

    drawText(app->renderer, fonts->baitSystem, app);
    drawText(app->renderer, fonts->echoSystem, app);
    drawText(app->renderer, fonts->trackingSystem, app);
    drawText(app->renderer, fonts->rebootAll, app);
}

void renderCustomgame(Game* game, App* app) {
    
}

void renderEndgame(Game* game, App* app) {
    SDL_SetRenderDrawColor(app->renderer, 2, 2, 2, 255);
    app->fonts->endGameText.textIn = "NIGHT " + std::to_string(game->currentNight);
    buildText(app->renderer, app->fonts->font1, app->fonts->endGameText);

    drawText(app->renderer, app->fonts->endGameText, app);
}
