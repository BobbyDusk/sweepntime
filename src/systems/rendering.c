#include "rendering.h"
#include "../assets.h"
#include "../components.h"

void RenderSystemInit(ecs_world_t *world, SDL_Renderer *renderer) {
  ECS_COMPONENT(world, Position);
  ECS_COMPONENT(world, Size);
  ECS_COMPONENT(world, Visualization);
  ecs_system(world, {.entity = ecs_entity(world, {.name = "RenderSystem",
                                                  .add = ecs_ids(ecs_dependson(EcsPostUpdate))}),
                     .query.terms = {{.id = ecs_id(Position), .inout = EcsIn},
                                     {.id = ecs_id(Size), .inout = EcsIn},
                                     {.id = ecs_id(Visualization), .inout = EcsIn}},
                     .callback = RenderSystem,
                     .ctx = renderer});
}

void RenderSystem(ecs_iter_t *it) {
  Position *positions = ecs_field(it, Position, 0);
  Size *sizes = ecs_field(it, Size, 1);
  Visualization *visualizations = ecs_field(it, Visualization, 2);
  SDL_Renderer *renderer = it->ctx;
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