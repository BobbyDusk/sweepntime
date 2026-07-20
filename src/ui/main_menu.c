#include "main_menu.h"
#include "../assets.h"
#include "../lib/clay.h"

void MainMenuUi() {
  CLAY(CLAY_ID("MainMenu"), {.layout = {.sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)}},
                             .backgroundColor = CLAY_COLOR(COLOR_WHITE)}) {
    CLAY(CLAY_ID("Title"), {.layout = {.sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(100)},
                                       .padding = CLAY_PADDING_ALL(16),
                                       .childGap = 16,
                                       .childAlignment = {.y = CLAY_ALIGN_Y_CENTER}},
                            .backgroundColor = {250, 250, 255, 255}}) {
      CLAY_TEXT(CLAY_STRING("Sweep'n Time"), {.fontSize = 48, .textColor = {0, 0, 0, 255}});
    }

    CLAY(CLAY_ID("Play"), {.layout = {.sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(100)},
                                      .padding = CLAY_PADDING_ALL(16),
                                      .childGap = 16,
                                      .childAlignment = {.y = CLAY_ALIGN_Y_CENTER}},
                           .backgroundColor = {250, 250, 255, 255}}) {
      CLAY_TEXT(CLAY_STRING("Play"), {.fontSize = 24, .textColor = {255, 255, 255, 255}});
    }
  }
}
