#pragma once

#include <SDL3/SDL.h>
#include <flecs.h>

void PhysicsSystemInit(ecs_world_t *world);
void PhysicsSystem(ecs_iter_t *it);