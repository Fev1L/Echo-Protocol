//
//  main.cpp
//  Echo Protocol
//
//  Created by Fev1L on 10.01.2026.
//

#define SDL_MAIN_USE_CALLBACKS

#include "types.h"
#include "render/render.h"
#include "logic/logic.h"
#include "scene/scene.h"

//=================================================================
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]){
    srand(time(0));
    App* app = new App();
    app->game = new Game();
    app->state = new State();
    app->fonts = new Font();
    Game* game = app->game;
    State* state = app->state;
    Font* fonts = app->fonts;
    
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Failure!");
        return SDL_APP_FAILURE;
    }
    
    if (!TTF_Init()) {
        SDL_Log("Failure!");
        return SDL_APP_FAILURE;
    }
    
    app->window = SDL_CreateWindow("Protocol 'ECHO'", 700, 700, SDL_WINDOW_FULLSCREEN);
    app->renderer = SDL_CreateRenderer(app->window, nullptr);
    
    SDL_GetWindowSize(app->window, &state->winW, &state->winH);
    
    std::string basePath = SDL_GetBasePath();
    std::string fontPath = basePath + "Assets/PressStart2P-Regular.ttf";
    std::cout<<fontPath<<std::endl;
    fonts->font1 = TTF_OpenFont(fontPath.c_str(), 15);
    if (!fonts->font1) {
        SDL_Log("FONT Failure!");
        return SDL_APP_FAILURE;
    }
    
    spawnMonster(game);
    game->noise.active = false;
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
        if (key == SDLK_A && game->viewAngleTarget <= 90) {
            game->targetView = ViewSide::LEFT;
            game->viewAngleTarget += 180.0f;
        }
        if (key == SDLK_D && game->viewAngleTarget >= -90) {
            game->targetView = ViewSide::RIGHT;
            game->viewAngleTarget -= 180.0f;
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
        for(int i = 0; i < game->GRID_H; i++){
            for(int j = 0; j < game->GRID_W; j++){
                int idx = i * game->GRID_W + j;
                if(idx < state->rooms.size() && isButtonClicked(state->rooms[idx].rect, mouseX, mouseY) && game->currentView == ViewSide::CENTER && game->system.baitSystem == true){
                    spawnNoise(game, j, i);
                }
            }
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
        
        if (isTextClicked(app->renderer, app->fonts->font1, game->menu.newGame, app, mouseX, mouseY)) {
            startNewGame(app);
        }
        if (isTextClicked(app->renderer, app->fonts->font1, game->menu.continueGame, app, mouseX, mouseY)) {
            loadGame(app);
        }
        if (isTextClicked(app->renderer, app->fonts->font1, game->menu.customGame, app, mouseX, mouseY))
            app->gamestate = GameState::CUSTOMGAME;
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
    
    if(app->gamestate == GameState::PLAYING){
        updateGameClock(game, app->deltaTime);
        updateCamera(game, app->deltaTime);
        updateNoise(game, app->deltaTime);
        updateEcho(game, app->deltaTime);
        checkEchoHit(game, app->deltaTime);
        updateMonster(game->monster, game, app->deltaTime);
        updateRepair(game, app->deltaTime);
    }
    if (app->gamestate == GameState::ENDSCREEN){
        game->nightIntroTimer += app->deltaTime;

        if (game->nightIntroTimer >= game->nightIntroDuration){
            app->gamestate = GameState::PLAYING;
        }
    }
    
    SDL_SetRenderDrawColor(app->renderer, 55, 55, 55, 255);
    SDL_RenderClear(app->renderer);
    app->state->rooms.clear();
    
    switch (app->gamestate) {
        case GameState::MENU:
            renderMenu(game, app);
            break;
        case GameState::PLAYING:
            renderGame(game, app);
            break;
        case GameState::CUSTOMGAME:
            renderCustomgame(game, app);
            break;
        case GameState::ENDSCREEN:
            renderEndgame(game, app);
            break;
    }
    
    if(game->monster.present && game->monster.x == game->centerX && game->monster.y == game->centerY)
        startNewGame(app);
    
    if (game->hours >= 8){
        game->currentNight++;
        saveProgress(app->game);
        loadGame(app);
    }
    
    SDL_RenderPresent(app->renderer);
    return SDL_APP_CONTINUE;
}
//=================================================================
void SDL_AppQuit(void* appstate, SDL_AppResult result){
    if (!appstate) return;
    
    App* app = (App*)appstate;
    if (!app) return;
    
    if (app->game) {
        if (app->renderer)
            SDL_DestroyRenderer(app->renderer);
        
        if (app->window)
            SDL_DestroyWindow(app->window);
        
        delete app->game;
    }
    
    if (app->state)
        delete app->state;
    
    if (app->fonts)
        delete app->fonts;
    
    TTF_Quit();
    SDL_Quit();
    
    delete app;
}
//=================================================================


