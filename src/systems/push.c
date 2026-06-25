#include "push.h"
#include "../components.h"
#include "../utils.h"
#include <math.h>

void PushSystemInit(ecs_world_t *world) {
  ECS_COMPONENT(world, Position);
  ECS_COMPONENT(world, Size);
  ECS_COMPONENT(world, Velocity);
  ECS_COMPONENT(world, ForceAccumulator);
  ECS_COMPONENT(world, PhysicsBody);
  ECS_SYSTEM(world, PushSystem,
             EcsPreUpdate, [in] Position, [in] Size, [in] Velocity,
             ForceAccumulator, [in] PhysicsBody);
}

void PushSystem(ecs_iter_t *it) {
  Position *position_src = ecs_field(it, Position, 0);
  Size *size_src = ecs_field(it, Size, 1);
  Velocity *velocity_src = ecs_field(it, Velocity, 2);
  PhysicsBody *physics_body_src = ecs_field(it, PhysicsBody, 3);

  ecs_query_t *movable_query = (ecs_query_t *)it->ctx;
  if (!movable_query)
    return;

  for (int i = 0; i < it->count; i++) {
    ecs_iter_t obj_it = ecs_query_iter(it->world, movable_query);

    while (ecs_query_next(&obj_it)) {
      Position *position_dst = ecs_field(&obj_it, Position, 0);
      Size *size_dst = ecs_field(&obj_it, Size, 1);
      Velocity *velocity_dst = ecs_field(&obj_it, Velocity, 2);
      ForceAccumulator *force_dst = ecs_field(&obj_it, ForceAccumulator, 3);
      PhysicsBody *physics_body_dst = ecs_field(&obj_it, PhysicsBody, 4);

      for (int j = 0; j < obj_it.count; j++) {

        // 1. CRITICAL: Avoid self-collision AND double-processing pairs
        // If Box 1 is compared to Box 2, we allow it.
        // When the loop reaches Box 2 compared to Box 1, we skip it!
        if (it->entities[i] >= obj_it.entities[j])
          continue;

        // Cache inverse weights
        float inv_weight_a = physics_body_src[i].inv_mass;
        float inv_weight_b = physics_body_dst[j].inv_mass;
        float total_inv_weight = inv_weight_a + inv_weight_b;

        // If BOTH objects have 0.0 inv_weight, they are both completely
        // unmovable walls
        if (total_inv_weight <= 0.0f)
          continue;

        if (CheckCollision(&position_src[i], &size_src[i], &position_dst[j],
                           &size_dst[j])) {
          // Compute Bounding Boxes
          float a_left = position_src[i].x;
          float a_right = position_src[i].x + size_src[i].w;
          float a_top = position_src[i].y;
          float a_bottom = position_src[i].y + size_src[i].h;

          float b_left = position_dst[j].x;
          float b_right = position_dst[j].x + size_dst[j].w;
          float b_top = position_dst[j].y;
          float b_bottom = position_dst[j].y + size_dst[j].h;

          float overlap_x =
              (a_left < b_left) ? (a_right - b_left) : (b_right - a_left);
          float overlap_y =
              (a_top < b_top) ? (a_bottom - b_top) : (b_bottom - a_top);

          // To determine who pushes whom, we see who is moving faster
          // relative to the other on the axis of collision
          if (overlap_x < overlap_y) {
            // X-Axis Collision
            // 1. Position Displacement split proportionally by weight
            float proportional_displacement_a =
                overlap_x * (inv_weight_a / total_inv_weight);
            float proportional_displacement_b =
                overlap_x * (inv_weight_b / total_inv_weight);

            if (a_left < b_left) {
              position_src[i].x -=
                  proportional_displacement_a; // Light object pushes left out
                                               // of the way
              position_dst[j].x +=
                  proportional_displacement_b; // Heavy object moves slightly
                                               // right
            } else {
              position_src[i].x += proportional_displacement_a;
              position_dst[j].x -= proportional_displacement_b;
            }

            // 2. Momentum Transfer (Shared Inelastic Velocity)
            float common_vx = (velocity_src[i].x * inv_weight_b +
                               velocity_dst[j].x * inv_weight_a) /
                              total_inv_weight;
            velocity_src[i].x = common_vx;
            velocity_dst[j].x = common_vx;
          } else {
            // Y-Axis Collision
            float proportional_displacement_a =
                overlap_y * (inv_weight_a / total_inv_weight);
            float proportional_displacement_b =
                overlap_y * (inv_weight_b / total_inv_weight);

            if (a_top < b_top) {
              position_src[i].y -= proportional_displacement_a;
              position_dst[j].y += proportional_displacement_b;
            } else {
              position_src[i].y += proportional_displacement_a;
              position_dst[j].y -= proportional_displacement_b;
            }

            float common_vy = (velocity_src[i].y * inv_weight_b +
                               velocity_dst[j].y * inv_weight_a) /
                              total_inv_weight;
            velocity_src[i].y = common_vy;
            velocity_dst[j].y = common_vy;
          }

          // Alert Flecs of changes
          ECS_COMPONENT(it->world, Position);
          ECS_COMPONENT(it->world, Velocity);
          ecs_modified(it->world, it->entities[i], Position);
          ecs_modified(it->world, it->entities[i], Velocity);
          ecs_modified(it->world, obj_it.entities[j], Position);
          ecs_modified(it->world, obj_it.entities[j], Velocity);
        }
      }
    }
  }
}