#include "physics.h"
#include "../components.h"

void PhysicsSystemInit(ecs_world_t *world) {
  // Initialize Box2D v3 with explicit top-down zero gravity
  b2WorldDef worldDef = b2DefaultWorldDef();
  worldDef.gravity = (b2Vec2){0.0F, 0.0F};
  b2WorldId b2_world = b2CreateWorld(&worldDef);

  // Register our global singleton context within the Flecs memory pool
  ECS_COMPONENT(world, PhysicsWorld);
  ecs_singleton_set(world, PhysicsWorld, {b2_world});
  ECS_SYSTEM(world, PhysicsSystem, EcsOnUpdate, [in] PhysicsWorld(PhysicsWorld));
}

void PhysicsSystem(ecs_iter_t *it) {
  // Grab our global physics world singleton
  PhysicsWorld *physics = ecs_field(it, PhysicsWorld, 0);

  if (it->delta_time > 0.0F) {
    b2World_Step(physics->world_id, it->delta_time, 4);
  }
}