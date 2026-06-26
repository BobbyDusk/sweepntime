#include "physics.h"
#include "components.h"
#include "systems/physics_step.h"
#include "systems/sync_physics_to_ecs.h"
#include <box2d/box2d.h>

void InitPhysics(ecs_world_t *ecs_world) {
  // Initialize Box2D v3 with explicit top-down zero gravity
  b2WorldDef worldDef = b2DefaultWorldDef();
  worldDef.gravity = (b2Vec2){0.0F, 0.0F};
  b2WorldId b2_world = b2CreateWorld(&worldDef);

  // Register our global singleton context within the Flecs memory pool
  ECS_COMPONENT(ecs_world, PhysicsWorld);
  ecs_singleton_set(ecs_world, PhysicsWorld, {b2_world});

  PhysicsStepSystemInit(ecs_world);
  SyncPhysicsToEcsSystemInit(ecs_world);
}