#include "debug.h"

void DebugSystemInit(ecs_world_t *world, SDL_Renderer *renderer) {
  ecs_system(world, {.entity = ecs_entity(world,
                                          {
                                              .name = "DebugSystem",
                                          }),
                     .phase = EcsPostUpdate,
                     .callback = DebugSystem,
                     .ctx = renderer});
}

void DebugSystem(ecs_iter_t *it) {
  // 1. Extract the SDL_Renderer context pointer from the Flecs world
  SDL_Renderer *renderer = it->ctx;
  if (!renderer) {
    return;
  }

  // 2. Rolling timer variables to stop the text from flickering frantically
  static int frame_count = 0;
  static float time_accumulator = 0.0F;
  static float displayed_fps = 0.0F;

  time_accumulator += it->delta_time;
  frame_count++;

  // Recalculate and refresh the display value every 0.5 seconds
  if (time_accumulator >= 0.5F) {
    displayed_fps = (float)frame_count / time_accumulator;
    frame_count = 0;
    time_accumulator = 0.0F;
  }

  // 3. Render text
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  SDL_RenderDebugTextFormat(renderer, 10.0F, 10.0F, "FPS: %.1f", displayed_fps);
}