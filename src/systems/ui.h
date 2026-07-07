#pragma once

#include <SDL3/SDL.h>
#include <flecs.h>

void UISystemInit(ecs_world_t *world, SDL_Renderer *renderer);
void UISystem(ecs_iter_t *it);