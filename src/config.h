#pragma once

// config
#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#define EXIT_ON_ESC_KEY true
#define PLAYER_WIDTH 50
#define PLAYER_HEIGHT 50
#define MOVEMENT_FORCE 4500
#define PLAYER_DAMPING 0.85f
#define PLAYER_INV_MASS 1.0f
#define REGULAR_FRICTION_COEFFICIENT 6.0f
#define SLIPPERY_FRICTION_COEFFICIENT 4.0f
#define STICKY_FRICTION_COEFFICIENT 7.5f

// constants
#define SQRT2 1.4142f
// all physics calculations are independant of framerate, however, the choice of parameters is
// based on a 60fps simulation, so we need to scale the damping factor based on the actual
// framerate.
#define BASE_FPS 60
