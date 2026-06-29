#pragma once

#include <SDL3/SDL.h>
#include <flecs.h>

void DebugSystemInit(ecs_world_t *world, SDL_Renderer *renderer);
void DebugSystem(ecs_iter_t *it);