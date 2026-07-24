#include "main_menu.h"
#include "../assets.h"
#include "../lib/clay.h"
#include "button.h"

// clang-format off
void MainMenuUi() {
  CLAY(CLAY_ID("MainMenu"), {
    .layout = {
      .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)}, 
      .layoutDirection = CLAY_TOP_TO_BOTTOM, 
      .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}, 
      .childGap = 16
    },
    .backgroundColor = CLAY_COLOR(BASE_COLOR_300)
    }
  ) {
    // TITLE
    CLAY_TEXT(CLAY_STRING("Sweep'n Time"), {
       .fontId = FONT_MAIN_ID, 
       .fontSize = 62, 
       .textColor = CLAY_COLOR(COLOR_BLACK)
       }
    );

    Button("Play");
    Button("Quit");
  }
}
// clang-format on
