#pragma once

#include <SDL3/SDL.h>
#include <flecs.h>

void InputSystemInit(ecs_world_t *world);
void InputSystem(ecs_iter_t *it);