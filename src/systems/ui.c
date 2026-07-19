#include "ui.h"
#include "../lib/clay.h"
#include "../ui_utils.h"
#include <clay_renderer.h>

void UISystemInit(ecs_world_t *world, SDL_Renderer *renderer, TTF_TextEngine *textEngine,
                  TTF_Font **myFonts) {
  Clay_SDL3RendererData rendererData = {
      .renderer = renderer, .textEngine = textEngine, .fonts = myFonts};

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

  const Clay_Color COLOR_LIGHT = (Clay_Color){224, 215, 210, 255};
  const Clay_Color COLOR_RED = (Clay_Color){168, 66, 28, 255};

  // An example of laying out a UI with a fixed width sidebar and flexible width main content
  CLAY(CLAY_ID("OuterContainer"), {.layout = {.sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)},
                                              .padding = CLAY_PADDING_ALL(16),
                                              .childGap = 16},
                                   .backgroundColor = {250, 250, 255, 255}}) {
    CLAY(CLAY_ID("SideBar"),
         {.layout = {.layoutDirection = CLAY_TOP_TO_BOTTOM,
                     .sizing = {.width = CLAY_SIZING_FIXED(300), .height = CLAY_SIZING_GROW(0)},
                     .padding = CLAY_PADDING_ALL(16),
                     .childGap = 16},
          .backgroundColor = COLOR_LIGHT}) {
      CLAY(CLAY_ID("ProfilePictureOuter"),
           {.layout = {.sizing = {.width = CLAY_SIZING_GROW(0)},
                       .padding = CLAY_PADDING_ALL(16),
                       .childGap = 16,
                       .childAlignment = {.y = CLAY_ALIGN_Y_CENTER}},
            .backgroundColor = COLOR_RED}) {
        CLAY_TEXT(CLAY_STRING("Clay - UI Library"),
                  {.fontSize = 24, .textColor = {255, 255, 255, 255}});
      }

      CLAY(CLAY_ID("MainContent"),
           {.layout = {.sizing = {.width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0)}},
            .backgroundColor = COLOR_LIGHT}) {}
    }
  }

  // 4. Run the layout math engine
  Clay_RenderCommandArray commands = Clay_EndLayout(it->delta_time);

  // Send it off to be drawn beautifully with rounded corners, text engines, and scissors!
  SDL_Clay_RenderClayCommands(rendererData, &commands);
}