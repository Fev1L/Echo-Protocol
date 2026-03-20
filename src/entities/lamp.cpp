//
// Created by Fev1L on 20.03.2026.
//

#include "entities.h"

void updateTopLamp(Game* game, float deltaTime) {
    FlickerLight& lamp = game->topLamp;
    lamp.timer += deltaTime;

    if (!lamp.flickering) {
        lamp.intensity = 0.9f + sin(SDL_GetTicks() * 0.003f) * 0.03f;

        if (lamp.timer >= lamp.nextFlicker) {
            lamp.timer = 0.0f;
            lamp.flickering = true;
            lamp.flickerDuration = 0.05f + (rand() % 20) / 100.0f;
        }
    } else {
        lamp.intensity = 0.45f + (rand() % 40) / 100.0f;
        lamp.flickerDuration -= deltaTime;

        if (lamp.flickerDuration <= 0.0f) {
            lamp.flickering = false;
            lamp.timer = 0.0f;
            lamp.nextFlicker = 1.5f + (rand() % 300) / 100.0f;
        }
    }

    lamp.intensity = SDL_clamp(lamp.intensity, 0.0f, 1.0f);
}