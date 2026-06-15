#pragma once

#include <flecs.h>

void MoveSystemInit(ecs_world_t *world);
void MoveSystem(ecs_iter_t *it);