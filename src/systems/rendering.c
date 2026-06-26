#include "rendering.h"
#include "../assets.h"
#include "../components.h"
#include "../config.h"

void RenderSystemInit(ecs_world_t *world, SDL_Renderer *renderer) {
  ECS_COMPONENT(world, Position);
  ECS_COMPONENT(world, RigidBody);
  ECS_COMPONENT(world, Visualization);
  ecs_system(world, {.entity = ecs_entity(world, {.name = "RenderSystem",
                                                  .add = ecs_ids(ecs_dependson(EcsPostUpdate))}),
                     .query.terms = {{.id = ecs_id(Position), .inout = EcsIn},
                                     {.id = ecs_id(RigidBody), .inout = EcsIn},
                                     {.id = ecs_id(Visualization), .inout = EcsIn}},
                     .callback = RenderSystem,
                     .ctx = renderer});
}

void RenderSystem(ecs_iter_t *it) {
  Position *positions = ecs_field(it, Position, 0);
  RigidBody *rigid_bodies = ecs_field(it, RigidBody, 1);
  Visualization *visualizations = ecs_field(it, Visualization, 2);
  SDL_Renderer *renderer = it->ctx;
  if (!renderer) {
    SDL_Log("Renderer is NULL in RenderSystem");
    return;
  }

  for (int i = 0; i < it->count; i++) {
    float x_screen_space = positions[i].x * PIXELS_PER_METER;
    float y_screen_space = positions[i].y * PIXELS_PER_METER;
    float width_screen_space = (rigid_bodies[i].half_width * 2.0F) * PIXELS_PER_METER;
    float height_screen_space = (rigid_bodies[i].half_height * 2.0F) * PIXELS_PER_METER;

    SDL_FRect rect = {x_screen_space - (width_screen_space / 2.0F),
                      y_screen_space - (height_screen_space / 2.0F), width_screen_space,
                      height_screen_space};
    SDL_SetRenderDrawColor(renderer, SDL_UNPACK_COLORS(visualizations[i].color));
    SDL_RenderFillRect(renderer, &rect);
  }
}