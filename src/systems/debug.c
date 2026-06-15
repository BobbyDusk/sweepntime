#include "debug.h"
#include <SDL3/SDL.h>
#include <flecs.h>

void DebugSystemInit(ecs_world_t *world) {
  ECS_SYSTEM(world, DebugSystem, EcsPostUpdate);
}

void DebugSystem(ecs_iter_t *it) {
  // 1. Extract the SDL_Renderer context pointer from the Flecs world
  SDL_Renderer *renderer = ecs_get_ctx(it->world);
  if (!renderer)
    return;

  // 2. Rolling timer variables to stop the text from flickering frantically
  static int frame_count = 0;
  static float time_accumulator = 0.0f;
  static float displayed_fps = 0.0f;

  time_accumulator += it->delta_time;
  frame_count++;

  // Recalculate and refresh the display value every 0.5 seconds
  if (time_accumulator >= 0.5f) {
    displayed_fps = (float)frame_count / time_accumulator;
    frame_count = 0;
    time_accumulator = 0.0f;
  }

  // 3. Render the text to the screen
  // Back up your renderer's current drawing color so you don't break other systems
  Uint8 r, g, b, a;
  SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

  // Set text color to bright green
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

  // Draw print-style formatted text at screen coordinates X: 10, Y: 10
  SDL_RenderDebugTextFormat(renderer, 10.0f, 10.0f, "FPS: %.1f", displayed_fps);

  // Restore the previous drawing color back to the renderer state
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
}