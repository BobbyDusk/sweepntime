#pragma once

#include <box2d/box2d.h>
#include <flecs.h>

void PhysicsSystemInit(ecs_world_t *world);
void PhysicsSystem(ecs_iter_t *it);