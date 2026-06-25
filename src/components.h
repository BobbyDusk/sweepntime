#pragma once

#include <SDL3/SDL.h>

typedef struct {
  float x;
  float y;
} Velocity;

typedef struct {
  float x;
  float y;
} ForceAccumulator;

typedef struct {
  float inv_mass; // 1.0 / mass. Use 0.0 for heavy, unmovable objects.
} PhysicsBody;

typedef struct {
  float friction_coefficient;
} SurfaceMaterial;

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
