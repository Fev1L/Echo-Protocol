//
//  main.cpp
//  Echo Protocol
//
//  Created by Fev1L on 10.01.2026.
//

#define SDL_MAIN_USE_CALLBACKS

#include "types.h"
#include "scene/scene.h"
#include "core/core.h"
#include "entities/entities.h"

//=================================================================
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]){
    srand(time(0));
    App* app = new App();
    app->game = new Game();
    app->state = new State();
    app->fonts = new Font();
    app->audio = new Audio();
    loadProgress(app->game);
    
    if(!initApp(app)){
        return SDL_APP_FAILURE;
    }
    spawnMonster(app->game);
    
    app->lastCounter = SDL_GetPerformanceCounter();
    app->deltaTime = 0.0;
    *appstate = app;
    return SDL_APP_CONTINUE;
}
//=================================================================
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event){
    App* app = (App*)appstate;
    Game* game = app->game;
    State* state = app->state;
    Font* fonts = app->fonts;
    
    float mouseX, mouseY;
    SDL_GetMouseState(&mouseX,&mouseY);
    if (game->system.active)
        return SDL_APP_CONTINUE;
    
    if (event->type == SDL_EVENT_QUIT){
        return SDL_APP_SUCCESS;
    }else if (event->type == SDL_EVENT_KEY_DOWN) {
        SDL_Keycode key = event->key.key;
        if (key == SDLK_ESCAPE)
            return SDL_APP_SUCCESS;
        if (key == SDLK_A && game->viewAngleTarget >= 0) {
            game->targetView = ViewSide::LEFT;
            game->viewAngleTarget -= 90.0f;
        }
        if (key == SDLK_D && game->viewAngleTarget <= 0) {
            game->targetView = ViewSide::RIGHT;
            game->viewAngleTarget += 90.0f;
        }
        if (key == SDLK_S) {
            game->targetView = ViewSide::CENTER;
            game->viewAngleTarget = 0.0f;
        }
        if (key == SDLK_T) {
            game->system.echoSystem = false;
            game->system.baitSystem = false;
            game->system.trackingSystem = false;
        }
        if (key == SDLK_Z) {
            game->currentNight++;
            saveProgress(app->game);
            loadGame(app);
        }
        if (key == SDLK_X) {
            game->currentNight--;
            saveProgress(app->game);
            loadGame(app);
        }
    }else if(event->type == SDL_EVENT_MOUSE_BUTTON_DOWN){
        if (game->currentView == ViewSide::CENTER && game->system.baitSystem)
        {
            if(state->rooms.empty()) return SDL_APP_CONTINUE;
            const SDL_FRect& first = state->rooms[0].rect;

            float cellW = first.w;
            float cellH = first.h;
            float gridStartX = first.x;
            float gridStartY = first.y;

            int j = (mouseX - gridStartX) / cellW;
            int i = (mouseY - gridStartY) / cellH;

            if (i >= 0 && i < game->GRID_H && j >= 0 && j < game->GRID_W) spawnNoise(game, j, i);
        }
        
        if(isTextClicked(app->renderer, fonts->font1, fonts->baitSystem, app, mouseX, mouseY) && game->system.baitSystem == false && game->currentView == ViewSide::RIGHT){
            game->system.active = true;
            game->system.timer = 0.0f;
            game->system.type = RepairType::BAIT;
        }
        if(isTextClicked(app->renderer, fonts->font1, fonts->echoSystem, app, mouseX, mouseY) && game->system.echoSystem == false && game->currentView == ViewSide::RIGHT){
            game->system.active = true;
            game->system.timer = 0.0f;
            game->system.type = RepairType::ECHO;
        }
        if(isTextClicked(app->renderer, fonts->font1, fonts->trackingSystem, app, mouseX, mouseY) && game->system.trackingSystem == false && game->currentView == ViewSide::RIGHT){
            game->system.active = true;
            game->system.timer = 0.0f;
            game->system.type = RepairType::TRACK;
        }
        if(isTextClicked(app->renderer, fonts->font1, fonts->rebootAll, app, mouseX, mouseY) && game->currentView == ViewSide::RIGHT){
            game->system.active = true;
            game->system.timer = -3.0f;
            game->system.type = RepairType::REBOOT;
        }
        
        if (isTextClicked(app->renderer, app->fonts->font2, game->menu.newGame, app, mouseX, mouseY)){
            startNewGame(app);
            SDL_PutAudioStreamData(app->audio->mouseClick.stream, app->audio->mouseClick.Data, app->audio->mouseClick.Len);
        }
        if (isTextClicked(app->renderer, app->fonts->font2, game->menu.continueGame, app, mouseX, mouseY)){
            loadGame(app);
            SDL_PutAudioStreamData(app->audio->mouseClick.stream, app->audio->mouseClick.Data, app->audio->mouseClick.Len);
        }
        if (isTextClicked(app->renderer, app->fonts->font2, game->menu.customGame, app, mouseX, mouseY) && game->currentNight){
            app->gamestate = GameState::CUSTOMGAME;
            SDL_PutAudioStreamData(app->audio->mouseClick.stream, app->audio->mouseClick.Data, app->audio->mouseClick.Len);
        }
    }
    
    return SDL_APP_CONTINUE;
}
//=================================================================
SDL_AppResult SDL_AppIterate(void* appstate){
    App* app = (App*)appstate;
    Game* game = app->game;
    
    Uint64 now = SDL_GetPerformanceCounter();
    app->deltaTime = (double)(now - app->lastCounter) / (double)SDL_GetPerformanceFrequency();
    app->lastCounter = now;
    
    if (game->monsters.size() < game->cfg.monsterCount) spawnMonster(game);
    
    if(app->gamestate == GameState::MENU){
        game->menu.menuFade += app->deltaTime * 1.5f;
        game->menu.menuFade = SDL_clamp(game->menu.menuFade, 0.0f, 1.0f);
    }
    if(app->gamestate == GameState::PLAYING){
        updateGameClock(game, app->deltaTime);
        updateCamera(game, app->deltaTime);
        updateNoises(game, app->deltaTime);
        updateEcho(game, app->deltaTime);
        checkEchoHit(game, app->deltaTime);
        updateMonsters(game, app->deltaTime);
        updateRepair(game, app->deltaTime);
    }
    if (app->gamestate == GameState::ENDSCREEN){
        game->nightIntroTimer += app->deltaTime;

        if (game->nightIntroTimer >= game->nightIntroDuration){
            app->gamestate = GameState::PLAYING;
        }
    }
    
    SDL_SetRenderDrawColor(app->renderer, 11, 16, 20, 255);
    SDL_RenderClear(app->renderer);
    
    switch (app->gamestate) {
        case GameState::MENU:
            renderMenu(game, app);
            break;
        case GameState::PLAYING:
            renderGame(game, app);
            for(auto& monster : game->monsters){
                if(monster.present && monster.x == game->centerX && monster.y == game->centerY){
                    saveProgress(app->game);
                    loadGame(app);
                }
            }
            break;
        case GameState::CUSTOMGAME:
            renderCustomgame(game, app);
            break;
        case GameState::ENDSCREEN:
            renderEndgame(game, app);
            break;
    }
    
    if (game->hours >= 8){
        game->currentNight++;
        saveProgress(app->game);
        loadGame(app);
    }

    // static float timer = 0;
    // static int frames = 0;
    //
    // timer += app->deltaTime;
    // frames++;
    //
    // if(timer >= 1.0f){
    //     SDL_Log("FPS: %d", frames);
    //     frames = 0;
    //     timer = 0;
    // }

    // SDL_Log("monsters: %zu", game->monsters.size());
    // SDL_Log("noise: %zu", game->noise.size());
    // SDL_Log("rooms: %zu", app->state->rooms.size());
    
    SDL_RenderPresent(app->renderer);
    return SDL_APP_CONTINUE;
}
//=================================================================
void SDL_AppQuit(void* appstate, SDL_AppResult result){
    if (!appstate) return;
    
    App* app = (App*)appstate;
    saveProgress(app->game);
    if (!app) return;
    
    if (app->game) {
        if (app->renderer)
            SDL_DestroyRenderer(app->renderer);
        
        if (app->window)
            SDL_DestroyWindow(app->window);
        
        delete app->game;
    }
    if (app->state) delete app->state;
    if (app->fonts) delete app->fonts;
    if (app->audio) delete app->audio;
    
    TTF_Quit();
    SDL_Quit();
    
    delete app;
}
//=================================================================
