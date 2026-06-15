#include "movement.h"
#include "../components.h"
#include "../config.h"

void MoveSystemInit(ecs_world_t *world) {
  ECS_COMPONENT(world, Position);
  ECS_COMPONENT(world, Movement);
  ECS_SYSTEM(world, MoveSystem, EcsOnUpdate, Position, Movement);
}

void MoveSystem(ecs_iter_t *it) {
  Position *positions = ecs_field(it, Position, 0);
  Movement *movement = ecs_field(it, Movement, 1);
  float delta_time = it->delta_time;

  for (int i = 0; i < it->count; i++) {
    switch (movement[i].direction) {
    case DIR_UP:
      positions[i].y -= MOVEMENT_SPEED * delta_time;
      break;
    case DIR_UP_RIGHT:
      positions[i].y -= MOVEMENT_SPEED * delta_time / SQRT2;
      positions[i].x += MOVEMENT_SPEED * delta_time / SQRT2;
      break;
    case DIR_RIGHT:
      positions[i].x += MOVEMENT_SPEED * delta_time;
      break;
    case DIR_DOWN_RIGHT:
      positions[i].y += MOVEMENT_SPEED * delta_time / SQRT2;
      positions[i].x += MOVEMENT_SPEED * delta_time / SQRT2;
      break;
    case DIR_DOWN:
      positions[i].y += MOVEMENT_SPEED * delta_time;
      break;
    case DIR_DOWN_LEFT:
      positions[i].y += MOVEMENT_SPEED * delta_time / SQRT2;
      positions[i].x -= MOVEMENT_SPEED * delta_time / SQRT2;
      break;
    case DIR_LEFT:
      positions[i].x -= MOVEMENT_SPEED * delta_time;
      break;
    case DIR_UP_LEFT:
      positions[i].y -= MOVEMENT_SPEED * delta_time / SQRT2;
      positions[i].x -= MOVEMENT_SPEED * delta_time / SQRT2;
      break;
    default:
      break;
    }
  }
}
