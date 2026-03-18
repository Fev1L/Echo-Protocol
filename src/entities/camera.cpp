//
// Created by Fev1L on 18.03.2026.
//

#include "entities.h"

void updateCamera(Game* game, float deltaTime) {
    float speed = 360.0f;

    float diff = game->viewAngleTarget - game->viewAngle;
    float step = speed * deltaTime;

    if (fabs(diff) <= step) {
        game->viewAngle = game->viewAngleTarget;
        game->currentView = game->targetView;
        game->camera.isTurning = false;
    } else {
        game->viewAngle += (diff > 0 ? step : -step);
        game->camera.isTurning = true;
    }

    if(game->viewAngle == 0)
        game->currentView = ViewSide::CENTER;
}