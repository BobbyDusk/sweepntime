#include "button.h"
#include "../assets.h"
#include "../lib/clay.h"
#include <string.h>

static struct Clay_Padding text_padding = {32, 32, 4, 4};

static char image_data[] = "background";

// clang-format off
void Button(const char *text) {
  Clay_String clay_str = {
    .chars = text,
    .isStaticallyAllocated = false,
    .length = (int)strlen(text)
  };

  CLAY(CLAY_SID(clay_str), {
    .cornerRadius = CLAY_CORNER_RADIUS(10),
    .layout = {
      .sizing = {CLAY_SIZING_FIT(), CLAY_SIZING_FIT()},
      .padding = text_padding,
      .childAlignment = {.y = CLAY_ALIGN_Y_CENTER}
    },
    .backgroundColor = Clay_Hovered() ? CLAY_COLOR(POS_ACCENT_600) : CLAY_COLOR(POS_ACCENT_800)
    }
  ) {
    CLAY_TEXT(clay_str, {
      .fontId = FONT_MAIN_ID, 
 
        .textColor = CLAY_COLOR(BASE_COLOR_100)
      }
    );
    CLAY_AUTO_ID( {
      .image = {
        .imageData = &image_data
      }
    }) {};
  };
}
// clang-format on
