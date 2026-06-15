#include "rendering.h"
#include "../assets.h"
#include "../components.h"

void RenderSystemInit(ecs_world_t *world) {
  ECS_COMPONENT(world, Position);
  ECS_COMPONENT(world, Size);
  ECS_COMPONENT(world, Visualization);
  ECS_SYSTEM(world, RenderSystem, EcsPostUpdate, Position, Size, Visualization);
}

void RenderSystem(ecs_iter_t *it) {
  Position *positions = ecs_field(it, Position, 0);
  Size *sizes = ecs_field(it, Size, 1);
  Visualization *visualizations = ecs_field(it, Visualization, 2);
  SDL_Renderer *renderer = ecs_get_ctx(it->world);
  if (!renderer) {
    SDL_Log("Renderer is NULL in RenderSystem");
    return;
  }

  for (int i = 0; i < it->count; i++) {
    SDL_FRect rect = {positions[i].x, positions[i].y, sizes[i].w, sizes[i].h};
    SDL_SetRenderDrawColor(renderer, SDL_UNPACK_COLORS(visualizations[i].color));
    SDL_RenderFillRect(renderer, &rect);
  }
}