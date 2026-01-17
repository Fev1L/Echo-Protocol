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

//=================================================================
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]){
    srand(time(0));
    App* app = new App();
    app->game = new Game();
    app->state = new State();
    Game* game = app->game;
    State* state = app->state;
    
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Failure!");
        return SDL_APP_FAILURE;
    }
    
    game->window = SDL_CreateWindow("Protocol 'ECHO'", 700, 700, SDL_WINDOW_FULLSCREEN);
    game->renderer = SDL_CreateRenderer(game->window, nullptr);
    
    SDL_GetWindowSize(game->window, &state->winW, &state->winH);
    
    state->table = {{layout(
        Anchor::TOP_LEFT,
        1.00f, 0.20f, //SIZE
        0.00f, 0.70f, //MARGIN
        app->state->winW,app->state->winH)},{143,103,33,255},0.0f,"TABLE"};
    state->monitor = {{layout(
        Anchor::TOP_LEFT,
        0.555f, 0.585f, //SIZE
        0.222f, 0.289f, //MARGIN
        state->winW,state->winH)},{0,0,0,255},0.0f,"Computer"};
    for(int i = 0; i < game->GRID_H; i++){
        for(int j = 0; j < game->GRID_W; j++){
            state->rooms.push_back(
                {{
                    layout(
                        Anchor::TOP_LEFT,
                        0.0069f, 0.0097f,
                        (0.239f + (0.0069f * j)),
                        (0.313f + (0.0097f * i)),
                        state->winW,
                        state->winH
                    )
                },
                {120,120,120,255},
                0.0f,
                "WEIGHT"}
            );
        }
    }
    
    if(rand() % 2){
        game->monster.x = rand() % game->GRID_W;
        game->monster.y = (rand() % 2) ? 0 : game->GRID_H - 1;
    }else{
        game->monster.y = rand() % game->GRID_H;
        game->monster.x = (rand() % 2) ? 0 : game->GRID_W - 1;
    }

    game->noise.active = false;
    game->noise.timeLeft = 0.0f;
    game->noise.cooldown = 60.0f;
    
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
    
    float mouseX, mouseY;
    SDL_GetMouseState(&mouseX,&mouseY);
    
    if (event->type == SDL_EVENT_QUIT){
        return SDL_APP_SUCCESS;
    }else if (event->type == SDL_EVENT_KEY_DOWN) {
        SDL_Keycode key = event->key.key;
        if (key == SDLK_ESCAPE) {
            return SDL_APP_SUCCESS;
        }
    }else if(event->type == SDL_EVENT_MOUSE_BUTTON_DOWN){
        for(int i = 0; i < game->GRID_H; i++){
            for(int j = 0; j < game->GRID_W; j++){
                int idx = i * game->GRID_W + j;
                if(idx < state->rooms.size() && isButtonClicked(state->rooms[idx].rect, mouseX, mouseY)){
                    game->noise.active = true;
                    game->noise.timeLeft = 10.0f;
                    game->noise.x = j;
                    game->noise.y = i;
                }
            }
        }
    }
    
    return SDL_APP_CONTINUE;
}
//=================================================================
SDL_AppResult SDL_AppIterate(void* appstate){
    App* app = (App*)appstate;
    Game* game = app->game;
    State* state = app->state;
    
    Uint64 now = SDL_GetPerformanceCounter();
    app->deltaTime = (double)(now - app->lastCounter) / (double)SDL_GetPerformanceFrequency();
    app->lastCounter = now;
    
    updateNoise(game, app->deltaTime);
    updateMonster(game->monster, game, app->deltaTime);
    
    SDL_SetRenderDrawColor(game->renderer, 55, 55, 55, 255);
    SDL_RenderClear(game->renderer);
    
    state->rooms.clear();
    for(int i = 0; i < game->GRID_H; i++){
        for(int j = 0; j < game->GRID_W; j++){

            SDL_Color color = {120,120,120,255};

            if (i == game->monster.y && j == game->monster.x) {
                color = {255, 0, 0, 255};
            }
            if (i == 28 && j == 38) {
                color = {0, 255, 0, 255};
            }
            state->rooms.push_back(
                {{
                    layout(
                        Anchor::TOP_LEFT,
                        0.0069f, 0.0097f,
                        (0.239f + (0.0069f * j)),
                        (0.313f + (0.0097f * i)),
                        state->winW,
                        state->winH
                    )
                },
                color,
                0.0f,
                "WEIGHT"}
            );
        }
    }
    
    drawRectangle(game->renderer, state->table);
    drawRectangle(game->renderer, state->monitor);
    for(Rectangle rec : state->rooms){
        drawRectangle(game->renderer, rec);
    }
    
    if(game->monster.x == game->centerX && game->monster.y == game->centerY){
        std::cout<<"GAME LOSE";
        return SDL_APP_SUCCESS;
    }
    
    SDL_RenderPresent(game->renderer);
    SDL_Delay(32);
    return SDL_APP_CONTINUE;
}
//=================================================================
void SDL_AppQuit(void* appstate, SDL_AppResult result){
    App* app = (App*)appstate;
    Game* game = app->game;
    
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
    delete game;
}
//=================================================================
