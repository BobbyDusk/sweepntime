#pragma once

#include "assets.h"
#include <SDL3/SDL.h>
#include <box2d/box2d.h>

typedef struct {
  b2BodyId body_id;
  float half_width;
  float half_height;
} RigidBody;

typedef struct {
  b2WorldId world_id;
} PhysicsWorld;

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
  Color color;
} Visualization;
