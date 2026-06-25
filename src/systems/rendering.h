#pragma once

#include <SDL3/SDL.h>
#include <flecs.h>

void RenderSystemInit(ecs_world_t *world, SDL_Renderer *renderer);
void RenderSystem(ecs_iter_t *it);
