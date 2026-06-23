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
  float damping;  // Linear friction (e.g., 0.98) so objects naturally slow down
} PhysicsBody;

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

typedef struct {
  float weight;
} Pushable;