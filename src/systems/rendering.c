#include "rendering.h"
#include "../assets.h"
#include "../components.h"

void RenderSystem(ecs_iter_t *it) {
  Position *positions = ecs_field(it, Position, 1);
  Size *sizes = ecs_field(it, Size, 2);
  SDL_Color *colors = ecs_field(it, SDL_Color, 3);
  SDL_Renderer *renderer = ecs_get_ctx(it->world);
  if (!renderer) {
    SDL_Log("Renderer is NULL in RenderSystem");
    return;
  }

  for (int i = 0; i < it->count; i++) {
    SDL_FRect rect = {positions[i].x, positions[i].y, sizes[i].w, sizes[i].h};
    SDL_SetRenderDrawColor(renderer, SDL_UNPACK_COLORS(colors[i]));
    SDL_RenderFillRect(renderer, &rect);
  }
}