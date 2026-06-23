#include "physics.h"
#include "../components.h"

void PhysicsSystemInit(ecs_world_t *world) {
  ECS_COMPONENT(world, Position);
  ECS_COMPONENT(world, Velocity);
  ECS_COMPONENT(world, ForceAccumulator);
  ECS_COMPONENT(world, PhysicsBody);
  ECS_SYSTEM(world, PhysicsSystem, EcsOnUpdate, Position, Velocity, ForceAccumulator, PhysicsBody);
}

void PhysicsSystem(ecs_iter_t *it) {
  Position *positions = ecs_field(it, Position, 0);
  Velocity *velocities = ecs_field(it, Velocity, 1);
  ForceAccumulator *force_accumulators = ecs_field(it, ForceAccumulator, 2);
  PhysicsBody *bodies = ecs_field(it, PhysicsBody, 3);
  float delta_time = it->delta_time;

  for (int i = 0; i < it->count; i++) {
    // Update velocity based on force accumulator and inverse mass
    velocities[i].x += force_accumulators[i].x * bodies[i].inv_mass * delta_time;
    velocities[i].y += force_accumulators[i].y * bodies[i].inv_mass * delta_time;

    // Apply damping to velocity
    velocities[i].x *= bodies[i].damping;
    velocities[i].y *= bodies[i].damping;

    // Update position based on velocity
    positions[i].x += velocities[i].x * delta_time;
    positions[i].y += velocities[i].y * delta_time;

    // Reset force accumulators for the next frame
    force_accumulators[i].x = 0.0f;
    force_accumulators[i].y = 0.0f;
  }
}