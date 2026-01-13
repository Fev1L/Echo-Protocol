//
//  main.cpp
//  Echo Protocol
//
//  Created by Fev1L on 10.01.2026.
//

#define SDL_MAIN_USE_CALLBACKS

#include "types.h"
#include "render/render.h"

//=================================================================
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]){
    App* app = new App();
    app->game = new Game();
    app->state = new State();
    
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Failure!");
        return SDL_APP_FAILURE;
    }
    
    app->game->window = SDL_CreateWindow("Protocol 'ECHO'", 700, 700, SDL_WINDOW_FULLSCREEN);
    app->game->renderer = SDL_CreateRenderer(app->game->window, nullptr);
    
    SDL_GetWindowSize(app->game->window, &app->state->winW, &app->state->winH);
    
    app->state->quit = {{layout(
        Anchor::TOP_LEFT,
        0.04f,
        0.06f,
        0.01f,
        0.01f,
        app->state->winW,app->state->winH)},{255,0,0,255},0.0f,"EXIT"};
    
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
        if(isButtonClicked(state->quit.rect, mouseX, mouseY)){
            return SDL_APP_SUCCESS;
        }
    }
    
    return SDL_APP_CONTINUE;
}
//=================================================================
SDL_AppResult SDL_AppIterate(void* appstate){
    App* app = (App*)appstate;
    Game* game = app->game;
    State* state = app->state;
    
    SDL_SetRenderDrawColor(game->renderer, 191, 191, 191, 255);
    SDL_RenderClear(game->renderer);
    
    drawRectangle(game->renderer, state->quit);
    
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
