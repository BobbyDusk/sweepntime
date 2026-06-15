#include "input.h"
#include "../components.h"

void InputSystem(ecs_iter_t *it) {
  Input *inputs = ecs_field(it, Input, 0);
  MoveDirection *directions = ecs_field(it, MoveDirection, 1);

  for (int i = 0; i < it->count; i++) {
    // Evaluate flags to determine the composite movement direction
    if (inputs[i].up && inputs[i].right) {
      directions[i] = DIR_UP_RIGHT;
    } else if (inputs[i].up) {
      directions[i] = DIR_UP;
    } else if (inputs[i].right) {
      directions[i] = DIR_RIGHT;
    } else {
      // If no keys are pressed, we can assign an idle/stop state
      directions[i] = DIR_NONE;
    }
  }
}