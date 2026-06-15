#include "input.h"
#include "../components.h"

void InputSystemInit(ecs_world_t *world) {
  ECS_COMPONENT(world, Input);
  ECS_COMPONENT(world, Movement);
  ECS_SYSTEM(world, InputSystem, EcsPreUpdate, Input, Movement);
}

void InputSystem(ecs_iter_t *it) {
  Input *inputs = ecs_field(it, Input, 0);
  Movement *movement = ecs_field(it, Movement, 1);

  for (int i = 0; i < it->count; i++) {
    // Evaluate flags to determine the composite movement direction
    if (inputs[i].up && inputs[i].right) {
      movement[i].direction = DIR_UP_RIGHT;
    } else if (inputs[i].down && inputs[i].right) {
      movement[i].direction = DIR_DOWN_RIGHT;
    } else if (inputs[i].down && inputs[i].left) {
      movement[i].direction = DIR_DOWN_LEFT;
    } else if (inputs[i].up && inputs[i].left) {
      movement[i].direction = DIR_UP_LEFT;
    } else if (inputs[i].up) {
      movement[i].direction = DIR_UP;
    } else if (inputs[i].right) {
      movement[i].direction = DIR_RIGHT;
    } else if (inputs[i].down) {
      movement[i].direction = DIR_DOWN;
    } else if (inputs[i].left) {
      movement[i].direction = DIR_LEFT;
    } else {
      movement[i].direction = DIR_NONE;
    }
  }
}