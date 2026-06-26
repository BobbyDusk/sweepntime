#include "input.h"
#include "../components.h"
#include "../config.h"

void InputSystemInit(ecs_world_t *world) {
  ECS_COMPONENT(world, Input);
  ECS_COMPONENT(world, RigidBody);
  ECS_SYSTEM(world, InputSystem, EcsPreUpdate, [in] Input, [in] RigidBody);
}

void InputSystem(ecs_iter_t *it) {
  Input *inputs = ecs_field(it, Input, 0);
  RigidBody *rigid_bodies = ecs_field(it, RigidBody, 1);
  b2Vec2 move_velocity = {0.0F, 0.0F};

  for (int i = 0; i < it->count; i++) {
    // Evaluate flags to determine the composite movement direction
    if ((int)inputs[i].up && (int)inputs[i].right) {
      move_velocity.x = MOVEMENT_SPEED / SQRT2;
      move_velocity.y = -MOVEMENT_SPEED / SQRT2;
    } else if ((int)inputs[i].down && (int)inputs[i].right) {
      move_velocity.x = MOVEMENT_SPEED / SQRT2;
      move_velocity.y = MOVEMENT_SPEED / SQRT2;
    } else if ((int)inputs[i].down && (int)inputs[i].left) {
      move_velocity.x = -MOVEMENT_SPEED / SQRT2;
      move_velocity.y = MOVEMENT_SPEED / SQRT2;
    } else if ((int)inputs[i].up && (int)inputs[i].left) {
      move_velocity.x = -MOVEMENT_SPEED / SQRT2;
      move_velocity.y = -MOVEMENT_SPEED / SQRT2;
    } else if (inputs[i].up) {
      move_velocity.x = 0.0F;
      move_velocity.y = -MOVEMENT_SPEED;
    } else if (inputs[i].right) {
      move_velocity.x = MOVEMENT_SPEED;
      move_velocity.y = 0.0F;
    } else if (inputs[i].down) {
      move_velocity.x = 0.0F;
      move_velocity.y = MOVEMENT_SPEED;
    } else if (inputs[i].left) {
      move_velocity.x = -MOVEMENT_SPEED;
      move_velocity.y = 0.0F;
    } else {
      move_velocity.x = 0.0F;
      move_velocity.y = 0.0F;
    }
    b2Body_SetLinearVelocity(rigid_bodies[i].body_id, move_velocity);
  }
}