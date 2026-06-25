#include "input.h"
#include "../components.h"
#include "../config.h"

void InputSystemInit(ecs_world_t *world) {
  ECS_COMPONENT(world, Input);
  ECS_COMPONENT(world, ForceAccumulator);
  ECS_SYSTEM(world, InputSystem, EcsPreUpdate, [in] Input, [out] ForceAccumulator);
}

void InputSystem(ecs_iter_t *it) {
  Input *inputs = ecs_field(it, Input, 0);
  ForceAccumulator *force_accumulators = ecs_field(it, ForceAccumulator, 1);

  for (int i = 0; i < it->count; i++) {
    // Evaluate flags to determine the composite movement direction
    if (inputs[i].up && inputs[i].right) {
      force_accumulators[i].x = MOVEMENT_FORCE / SQRT2;
      force_accumulators[i].y = -MOVEMENT_FORCE / SQRT2;
    } else if (inputs[i].down && inputs[i].right) {
      force_accumulators[i].x = MOVEMENT_FORCE / SQRT2;
      force_accumulators[i].y = MOVEMENT_FORCE / SQRT2;
    } else if (inputs[i].down && inputs[i].left) {
      force_accumulators[i].x = -MOVEMENT_FORCE / SQRT2;
      force_accumulators[i].y = MOVEMENT_FORCE / SQRT2;
    } else if (inputs[i].up && inputs[i].left) {
      force_accumulators[i].x = -MOVEMENT_FORCE / SQRT2;
      force_accumulators[i].y = -MOVEMENT_FORCE / SQRT2;
    } else if (inputs[i].up) {
      force_accumulators[i].x = 0.0F;
      force_accumulators[i].y = -MOVEMENT_FORCE;
    } else if (inputs[i].right) {
      force_accumulators[i].x = MOVEMENT_FORCE;
      force_accumulators[i].y = 0.0F;
    } else if (inputs[i].down) {
      force_accumulators[i].x = 0.0F;
      force_accumulators[i].y = MOVEMENT_FORCE;
    } else if (inputs[i].left) {
      force_accumulators[i].x = -MOVEMENT_FORCE;
      force_accumulators[i].y = 0.0F;
    } else {
      force_accumulators[i].x = 0.0F;
      force_accumulators[i].y = 0.0F;
    }
  }
}