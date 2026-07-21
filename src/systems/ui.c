#include "ui.h"
#include "../lib/clay.h"
#include "../ui_utils.h"
#include "ui/main_menu.h"
#include <clay_renderer.h>

void UISystemInit(ecs_world_t *world, SDL_Renderer *renderer, TTF_TextEngine *text_engine,
                  TTF_Font **my_fonts) {
  static Clay_SDL3RendererData rendererData;
  rendererData.renderer = renderer;
  rendererData.textEngine = text_engine;
  rendererData.fonts = my_fonts;

  ecs_system(world, {.entity = ecs_entity(world, {.name = "UISystem"}),
                     .phase = EcsPostUpdate,
                     .callback = UISystem,
                     .ctx = &rendererData});
}

void UISystem(ecs_iter_t *it) {
  Clay_SDL3RendererData *rendererData = (Clay_SDL3RendererData *)it->ctx;

  // 1. Sync mouse coordinates
  UpdateUIInput();

  // 2. Begin declaring your layout
  Clay_BeginLayout();

  MainMenuUi();

  // 4. Run the layout math engine
  Clay_RenderCommandArray commands = Clay_EndLayout(it->delta_time);

  // Send it off to be drawn beautifully with rounded corners, text engines, and scissors!
  SDL_Clay_RenderClayCommands(rendererData, &commands);
}
