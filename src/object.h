#pragma once

#include <SDL3/SDL.h>
#include <box2d/box2d.h>
#include <flecs.h>

typedef struct {
  float density;
  float friction;
  float linear_damping;
} ObjectPhysicsParams;

ObjectPhysicsParams DefaultObjectPhysicsParams(void);

ecs_entity_t CreateObject(ecs_world_t *ecs_world, float x, float y, float width, float height,
                          bool movable, SDL_Color color,
                          const ObjectPhysicsParams *physics_params_ptr);