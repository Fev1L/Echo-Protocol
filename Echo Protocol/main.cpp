//
//  main.cpp
//  Echo Protocol
//
//  Created by Fev1L on 10.01.2026.
//

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <cstdlib>

#include "types.h"

//=================================================================
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]){
    Game* game = new Game();
    
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Failure!");
        return SDL_APP_FAILURE;
    }
    
    game->window = SDL_CreateWindow("Minesweeper by Fev1L v.1", 550, 700, SDL_WINDOW_OPENGL );
    game->renderer = SDL_CreateRenderer(game->window, nullptr);
    
    *appstate = game;
    return SDL_APP_CONTINUE;
}
//=================================================================
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event){
    Game* game = (Game*)appstate;
    
    return SDL_APP_CONTINUE;
}
//=================================================================
SDL_AppResult SDL_AppIterate(void* appstate){
    Game* game = (Game*)appstate;
    
    return SDL_APP_CONTINUE;
}
//=================================================================
void SDL_AppQuit(void* appstate, SDL_AppResult result){
    Game* game = (Game*)appstate;
    
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
    delete game;
}
//=================================================================
