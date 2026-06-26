#pragma once

#include <box2d/box2d.h>
#include <flecs.h>

void PhysicsStepSystemInit(ecs_world_t *world);
void PhysicsStepSystem(ecs_iter_t *it);