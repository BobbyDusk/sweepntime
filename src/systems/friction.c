// friction_system.c
#include "friction.h"
#include "../components.h"
#include "../config.h"
#include "../utils.h"
#include <flecs.h>

void FrictionSystemInit(ecs_world_t *world, ecs_query_t *surfaces_query) {
  ECS_COMPONENT(world, Position);
  ECS_COMPONENT(world, Size);
  ECS_COMPONENT(world, Velocity);
  ECS_COMPONENT(world, ForceAccumulator);
  ecs_system(world, {.entity = ecs_entity(world, {.name = "FrictionSystem",
                                                  .add = ecs_ids(ecs_dependson(EcsPreUpdate))}),
                     .query.terms = {{.id = ecs_id(Position), .inout = EcsIn},
                                     {.id = ecs_id(Size), .inout = EcsIn},
                                     {.id = ecs_id(Velocity), .inout = EcsIn},
                                     {.id = ecs_id(ForceAccumulator)}},
                     .callback = FrictionSystem,
                     .ctx = surfaces_query});
}

void FrictionSystem(ecs_iter_t *it) {
  Position *positions = ecs_field(it, Position, 0);
  Size *sizes = ecs_field(it, Size, 1);
  Velocity *velocities = ecs_field(it, Velocity, 2);
  ForceAccumulator *force_accumulators = ecs_field(it, ForceAccumulator, 3);
  ecs_query_t *surfaces_query = it->ctx;
  if (!surfaces_query) {
    SDL_Log("Surfaces query is NULL in FrictionSystem");
    return;
  }

  for (int i = 0; i < it->count; i++) {
    float surface_friction = REGULAR_FRICTION_COEFFICIENT;

    ecs_iter_t surface_it = ecs_query_iter(it->world, surfaces_query);
    while (ecs_query_next(&surface_it)) {
      Position *surface_positions = ecs_field(&surface_it, Position, 0);
      Size *surface_sizes = ecs_field(&surface_it, Size, 1);
      SurfaceMaterial *surface_materials = ecs_field(&surface_it, SurfaceMaterial, 2);

      for (int j = 0; j < surface_it.count; j++) {
        if (CheckCollision(&positions[i], &sizes[i], &surface_positions[j], &surface_sizes[j])) {
          surface_friction = surface_materials[j].friction_coefficient;
          break; // Exit the loop once a collision is detected
        }
      }
    }

    // Friction force is opposite to the direction of travel
    float friction_x = -velocities[i].x * surface_friction;
    float friction_y = -velocities[i].y * surface_friction;

    // Accumulate the friction force directly into the frame calculation
    force_accumulators[i].x += friction_x;
    force_accumulators[i].y += friction_y;
  }
}