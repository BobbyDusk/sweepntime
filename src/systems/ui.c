#include "ui.h"
#include "../lib/clay.h"
#include "../ui_utils.h"
#include "ui/main_menu.h"
#include <clay_renderer.h>

void UISystemInit(ecs_world_t *world, SDL_Renderer *renderer, TTF_TextEngine *text_engine, TTF_Font **my_fonts) {
  static Clay_SDL3RendererData renderer_data;
  renderer_data.renderer = renderer;
  renderer_data.textEngine = text_engine;
  renderer_data.fonts = my_fonts;

  ecs_system(world, {.entity = ecs_entity(world, {.name = "UISystem"}), .phase = EcsPostUpdate, .callback = UISystem, .ctx = &renderer_data});
}

void UISystem(ecs_iter_t *it) {
  Clay_SDL3RendererData *renderer_data = (Clay_SDL3RendererData *)it->ctx;

  // 1. Sync mouse coordinates
  UpdateUIInput(renderer_data->renderer);

  // 2. Begin declaring your layout
  Clay_BeginLayout();

  MainMenuUi();

  // 4. Run the layout math engine
  Clay_RenderCommandArray commands = Clay_EndLayout(it->delta_time);

  SDL_Clay_RenderClayCommands(renderer_data, &commands);
}
