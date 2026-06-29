#pragma once

// config
#define SDL_MAIN_USE_CALLBACKS 1 // use the callbacks instead oF main()
#define EXIT_ON_ESC_KEY true
#define PIXELS_PER_METER 100.0F // 1920 screen width is roughtly 20 meters
#define PLAYER_WIDTH 0.5        // in m
#define PLAYER_HEIGHT 0.5       // in m
#define MOVEMENT_SPEED 6.0F     // in m/s

// TODO: remove all old physics config
#define MOVEMENT_FORCE 4500
#define PLAYER_DAMPING 0.85F
#define PLAYER_INV_MASS 1.0F
#define REGULAR_FRICTION_COEFFICIENT 6.0F
#define SLIPPERY_FRICTION_COEFFICIENT 1.0F
#define STICKY_FRICTION_COEFFICIENT 20.0F

// constants
#define SQRT2 1.4142F
// all physics calculations are independant of Framerate, however, the choice oF
// parameters is based on a 60fps simulation, so we need to scale the damping
// factor based on the actual Framerate.
#define BASE_FPS 60.0F
#define BASE_SCREEN_WIDTH 1920.0F
#define BASE_SCREEN_HEIGHT 1080.0F
#define DEFAULT_DENSITY 1.0F // kg per m^2
#define DEFAULT_FRICTION 5.0F
#define DEFAULT_LINEAR_DAMPING 20.0F
