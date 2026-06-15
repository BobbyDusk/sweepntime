#pragma once

#include <SDL3/SDL.h>

typedef enum {
  DIR_UP,
  DIR_DOWN,
  DIR_LEFT,
  DIR_RIGHT,
  DIR_UP_RIGHT,
  DIR_DOWN_RIGHT,
  DIR_DOWN_LEFT,
  DIR_UP_LEFT,
  DIR_NONE
} MoveDirection;

typedef struct {
  MoveDirection direction;
} Movement;

typedef struct {
  bool up;
  bool down;
  bool left;
  bool right;
} Input;

typedef struct {
  SDL_Color color;
} Visualization;

typedef struct {
  float x;
  float y;
} Position;

typedef struct {
  float w;
  float h;
} Size;
