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
    app->font = new Font();
    Game* game = app->game;
    State* state = app->state;
    Font* fonts = app->font;
    
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Failure!");
        return SDL_APP_FAILURE;
    }
    
    if (!TTF_Init()) {
        SDL_Log("Failure!");
        return SDL_APP_FAILURE;
    }
    
    game->window = SDL_CreateWindow("Protocol 'ECHO'", 700, 700, SDL_WINDOW_FULLSCREEN);
    game->renderer = SDL_CreateRenderer(game->window, nullptr);
    
    SDL_GetWindowSize(game->window, &state->winW, &state->winH);
    
    std::string basePath = SDL_GetBasePath();
    std::string fontPath = basePath + "Assets/PressStart2P-Regular.ttf";
    std::cout<<fontPath<<std::endl;
    fonts->font1 = TTF_OpenFont(fontPath.c_str(), 15);
    if (!fonts->font1) {
        SDL_Log("FONT Failure!");
        SDL_DestroyRenderer(app->game->renderer);
        SDL_DestroyWindow(app->game->window);
        TTF_Quit();
        SDL_Quit();
        return SDL_APP_FAILURE;
    }
    
    state->table = {{layout(
        Anchor::TOP_LEFT,
        1.00f, 0.20f, //SIZE
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
                   "WEIGHT",
                   ViewSide::CENTER
               }
            );
        }
    }
    fonts->night = {{layoutText(0.006f, 0.009f, state->winW, state->winH)}, {255,255,255,255},"Night", "NIGHT 1"};
    
    state->tableR = {{layout(
        Anchor::TOP_LEFT,
        1.00f, 0.20f, //SIZE
        0.00f, 0.70f, //MARGIN
        app->state->winW,app->state->winH)},{143,103,33,255},"TABLE",ViewSide::RIGHT};
    
    if(rand() % 2){
        game->monster.x = rand() % game->GRID_W;
        game->monster.y = (rand() % 2) ? 0 : game->GRID_H - 1;
    }else{
        game->monster.y = rand() % game->GRID_H;
        game->monster.x = (rand() % 2) ? 0 : game->GRID_W - 1;
    }

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
    
    float mouseX, mouseY;
    SDL_GetMouseState(&mouseX,&mouseY);
    
    if (event->type == SDL_EVENT_QUIT){
        return SDL_APP_SUCCESS;
    }else if (event->type == SDL_EVENT_KEY_DOWN) {
        SDL_Keycode key = event->key.key;
        if (key == SDLK_ESCAPE)
            return SDL_APP_SUCCESS;
        if (key == SDLK_A && game->viewAngleTarget <= 180) {
            game->targetView = ViewSide::LEFT;
            game->viewAngleTarget += 180.0f;
        }
        if (key == SDLK_D && game->viewAngleTarget <= 180) {
            game->targetView = ViewSide::RIGHT;
            game->viewAngleTarget -= 180.0f;
        }
        if (key == SDLK_S) {
            game->targetView = ViewSide::CENTER;
            game->viewAngleTarget = 0.0f;
        }
    }else if(event->type == SDL_EVENT_MOUSE_BUTTON_DOWN){
        for(int i = 0; i < game->GRID_H; i++){
            for(int j = 0; j < game->GRID_W; j++){
                int idx = i * game->GRID_W + j;
                if(idx < state->rooms.size() && isButtonClicked(state->rooms[idx].rect, mouseX, mouseY)){
                    spawnNoise(game, j, i);
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
    Font* fonts = app->font;
    
    Uint64 now = SDL_GetPerformanceCounter();
    app->deltaTime = (double)(now - app->lastCounter) / (double)SDL_GetPerformanceFrequency();
    app->lastCounter = now;
    updateGameClock(game, app->deltaTime);
    
    updateCamera(game, app->deltaTime);
    updateEcho(game, app->deltaTime);
    checkEchoHit(game, app->deltaTime);
    updateNoise(game, app->deltaTime);
    updateMonster(game->monster, game, app->deltaTime);
    
    SDL_SetRenderDrawColor(game->renderer, 55, 55, 55, 255);
    SDL_RenderClear(game->renderer);
    
    state->rooms.clear();
    for(int i = 0; i < game->GRID_H; i++){
        for(int j = 0; j < game->GRID_W; j++){

            SDL_Color color = {120,120,120,255};

            if (game->noise.active) {
                int dx = j - game->noise.x;
                int dy = i - game->noise.y;
                float dist = sqrtf(dx * dx + dy * dy);

                if (dist <= game->noise.radius) {
                    float t = 1.0f - (dist / game->noise.radius);
                    t = SDL_clamp(t, 0.0f, 1.0f);

                    Uint8 r = (Uint8)(120 + t * 135);
                    Uint8 g = (Uint8)(120 + t * 135);
                    Uint8 b = (Uint8)(120 - t * 120);

                    color = { r, g, b, 255 };
                }
            }
            
            if (game->echo.active) {
                int dx = j - game->centerX;
                int dy = i - game->centerY;
                float dist = sqrtf(dx*dx + dy*dy);

                if (fabs(dist - game->echo.radius) < 0.5f) {
                    color = {255, 255, 255, 180};
                }
            }

            if (game->monster.visible &&
                i == game->monster.y &&
                j == game->monster.x) {
                color = {255, 80, 80, 255};
            }

            if (i == game->centerY && j == game->centerX) {
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
                   "WEIGHT",
                   ViewSide::CENTER
               }
            );
        }
    }
    
    drawRectangle(game->renderer, state->table, app);
    drawRectangle(game->renderer, state->tableR, app);
    drawRectangle(game->renderer, state->monitor, app);
    for(Rectangle rec : state->rooms){
        drawRectangle(game->renderer, rec, app);
    }
    drawText(game->renderer, fonts->font1, fonts->night);
    
    std::ostringstream ss;
    ss << std::setw(2) << std::setfill('0') << game->hours
       << ":"
       << std::setw(2) << std::setfill('0') << game->minutes;

    fonts->hours = {{layoutText(0.006f, 0.029f, state->winW, state->winH)}, {255,255,255,255},"Hours", ss.str()};
    drawText(game->renderer, fonts->font1, fonts->hours);

    std::ostringstream bait_ss;
    bait_ss << std::setw(2) << std::setfill('0') << static_cast<int>(game->noise.cooldown);
    fonts->bait = {{layoutText(0.234f, 0.183f, state->winW, state->winH)}, {255,255,255,255},"Bait", bait_ss.str()};
    drawText(game->renderer, fonts->font1, fonts->bait);
    
    if(game->monster.x == game->centerX && game->monster.y == game->centerY){
        std::cout<<"GAME LOSE";
        return SDL_APP_SUCCESS;
    }
    if (game->hours >= 8) {
        std::cout << "PLAYER SURVIVED THE NIGHT\n";
        return SDL_APP_SUCCESS;
    }
    
    SDL_RenderPresent(game->renderer);
    SDL_Delay(32);
    return SDL_APP_CONTINUE;
}
//=================================================================
void SDL_AppQuit(void* appstate, SDL_AppResult result){
    if (!appstate) return;

        App* app = (App*)appstate;
        if (!app) return;

        if (app->game) {
            if (app->game->renderer)
                SDL_DestroyRenderer(app->game->renderer);

            if (app->game->window)
                SDL_DestroyWindow(app->game->window);

            delete app->game;
        }

        if (app->state)
            delete app->state;

        if (app->font)
            delete app->font;

        TTF_Quit();
        SDL_Quit();

        delete app;}
//=================================================================

