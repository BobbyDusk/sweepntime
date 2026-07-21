#include "main_menu.h"
#include "../assets.h"
#include "../lib/clay.h"

struct Clay_Padding text_padding = {32, 32, 4, 4};

void MainMenuUi() {
  CLAY(CLAY_ID("MainMenu"),
       {.layout = {.sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)},
                   .layoutDirection = CLAY_TOP_TO_BOTTOM,
                   .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER},
                   .childGap = 16},
        .backgroundColor = CLAY_COLOR(BASE_COLOR_300)}) {

    // TITLE
    CLAY_TEXT(CLAY_STRING("Sweep'n Time"),
              {.fontId = FONT_MAIN_ID, .fontSize = 62, .textColor = CLAY_COLOR(COLOR_BLACK)});

    // PLAY BUTTON
    CLAY(CLAY_ID("Play"), {.cornerRadius = CLAY_CORNER_RADIUS(10),
                           .layout = {.sizing = {CLAY_SIZING_FIT(), CLAY_SIZING_FIT()},
                                      .padding = text_padding,
                                      .childGap = 16,
                                      .childAlignment = {.y = CLAY_ALIGN_Y_CENTER}},
                           .backgroundColor = CLAY_COLOR(POS_ACCENT_800)}) {
      CLAY_TEXT(CLAY_STRING("Play"),
                {.fontId = FONT_MAIN_ID, .fontSize = 48, .textColor = CLAY_COLOR(BASE_COLOR_100)});
    }

    // QUIT BUTTON
    CLAY(CLAY_ID("Quit"), {.cornerRadius = CLAY_CORNER_RADIUS(10),
                           .layout = {.sizing = {CLAY_SIZING_FIT(), CLAY_SIZING_FIT()},
                                      .padding = text_padding,
                                      .childGap = 16,
                                      .childAlignment = {.y = CLAY_ALIGN_Y_CENTER}},
                           .backgroundColor = CLAY_COLOR(EVIL_COLOR_800)}) {
      CLAY_TEXT(CLAY_STRING("Quit"),
                {.fontId = FONT_MAIN_ID, .fontSize = 48, .textColor = CLAY_COLOR(BASE_COLOR_100)});
    }
  }
}
