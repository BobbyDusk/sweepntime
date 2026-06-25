#pragma once

#include <SDL3/SDL.h>
#include <flecs.h>

void FrictionSystemInit(ecs_world_t *world, ecs_query_t *surfaces_query);
void FrictionSystem(ecs_iter_t *it);