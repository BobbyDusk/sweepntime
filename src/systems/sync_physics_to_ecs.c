#include "sync_physics_to_ecs.h"

#include "../components.h"
#include "../config.h"
#include <box2d/box2d.h>

void SyncPhysicsToEcsSystemInit(ecs_world_t *world) {
  ECS_COMPONENT(world, Position);
  ECS_COMPONENT(world, RigidBody);
  ECS_SYSTEM(world, SyncPhysicsToEcsSystem, EcsOnUpdate, [out] Position, [in] RigidBody);
}

void SyncPhysicsToEcsSystem(ecs_iter_t *it) {
  Position *positions = ecs_field(it, Position, 0);
  RigidBody *rigid_bodies = ecs_field(it, RigidBody, 1);

  for (int i = 0; i < it->count; i++) {
    // Query Box2D for the raw physics coordinates (Meters)
    b2Vec2 b2_pos = b2Body_GetPosition(rigid_bodies[i].body_id);

    // Convert to Pixel Coordinates for SDL3 Rendering
    positions[i].x = b2_pos.x * PIXELS_PER_METER;
    positions[i].y = b2_pos.y * PIXELS_PER_METER;
  }
}
