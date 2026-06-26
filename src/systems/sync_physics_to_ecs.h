#pragma once

#include <flecs.h>

void SyncPhysicsToEcsSystemInit(ecs_world_t *world);
void SyncPhysicsToEcsSystem(ecs_iter_t *it);