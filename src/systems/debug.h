#pragma once

#include <flecs.h>

void DebugSystemInit(ecs_world_t *world);
void DebugSystem(ecs_iter_t *it);