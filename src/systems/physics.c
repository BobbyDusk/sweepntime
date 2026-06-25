#include "physics.h"
#include "../components.h"
#include "../config.h"
#include <math.h>

void PhysicsSystemInit(ecs_world_t *world) {
  ECS_COMPONENT(world, Position);
  ECS_COMPONENT(world, Velocity);
  ECS_COMPONENT(world, ForceAccumulator);
  ECS_COMPONENT(world, PhysicsBody);
  ECS_SYSTEM(world, PhysicsSystem, EcsOnUpdate, Position, Velocity,
             ForceAccumulator, [in] PhysicsBody);
}

void PhysicsSystem(ecs_iter_t *it) {
  Position *positions = ecs_field(it, Position, 0);
  Velocity *velocities = ecs_field(it, Velocity, 1);
  ForceAccumulator *force_accumulators = ecs_field(it, ForceAccumulator, 2);
  PhysicsBody *bodies = ecs_field(it, PhysicsBody, 3);
  float delta_time = it->delta_time;

  for (int i = 0; i < it->count; i++) {
    // a = F / m or a = F * inv_mass
    float acceleration_x = force_accumulators[i].x * bodies[i].inv_mass;
    float acceleration_y = force_accumulators[i].y * bodies[i].inv_mass;

    // v = v + a * dt
    velocities[i].x += acceleration_x * delta_time;
    velocities[i].y += acceleration_y * delta_time;

    // Update position based on velocity
    positions[i].x += velocities[i].x * delta_time;
    positions[i].y += velocities[i].y * delta_time;

    // Reset force accumulators for the next frame
    force_accumulators[i].x = 0.0f;
    force_accumulators[i].y = 0.0f;
  }
}