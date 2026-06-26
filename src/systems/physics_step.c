#include "physics_step.h"
#include "../components.h"

void PhysicsStepSystemInit(ecs_world_t *world) {
  ECS_COMPONENT(world, PhysicsWorld);
  ECS_SYSTEM(world, PhysicsStepSystem, EcsOnUpdate, [in] PhysicsWorld(PhysicsWorld));
}

void PhysicsStepSystem(ecs_iter_t *it) {
  // Grab our global physics world singleton
  PhysicsWorld *physics = ecs_field(it, PhysicsWorld, 0);

  if (it->delta_time > 0.0F) {
    // Box2D v3 introduces sub-stepping. 4 is the recommended default.
    b2World_Step(physics->world_id, it->delta_time, 4);
  }
}