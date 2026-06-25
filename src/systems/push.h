#pragma once

#include "flecs.h"

void PushSystemInit(ecs_world_t *world);
void PushSystem(ecs_iter_t *it);