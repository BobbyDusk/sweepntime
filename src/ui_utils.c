#include "ui_utils.h"
#include "clay.h"
#include <SDL3/SDL.h>
#include <clay_renderer.h> // Include your new renderer header

// We store a global pointer to the font array so the measurement callback can see it
static TTF_Font **g_UIFonts = NULL;

void HandleClayError(Clay_ErrorData errorData) {
  SDL_Log("Clay Error: %s\n", errorData.errorText.chars);
}

// Clay calls this during layout to see how big a bounding box needs to be
Clay_Dimensions MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config,
                            [[maybe_unused]] void *userData) {
  if (!g_UIFonts) {
    return (Clay_Dimensions){0.0F, 0.0F};
  }

  // Look up the exact font chosen by config->fontId, matching your new renderer
  TTF_Font *font = g_UIFonts[config->fontId];
  if (!font) {
    return (Clay_Dimensions){0.0F, 0.0F};
  }

  // Adjust font size to match Clay's requested configuration
  TTF_SetFontSize(font, (float)config->fontSize);

  int width = 0;
  int height = 0;
  if (!TTF_GetStringSize(font, text.chars, text.length, &width, &height)) {
    SDL_Log("SDL3_ttf text measurement error: %s", SDL_GetError());
  }

  return (Clay_Dimensions){(float)width, (float)height};
}

// Pass your fonts array here so Clay can measure them accurately
void InitUI(int window_width, int window_height, TTF_Font **fontsArray) {
  g_UIFonts = fontsArray;

  uint64_t memory_size = Clay_MinMemorySize();
  Clay_Arena ui_arena = Clay_CreateArenaWithCapacityAndMemory(memory_size, malloc(memory_size));

  Clay_Initialize(ui_arena, (Clay_Dimensions){(float)window_width, (float)window_height},
                  (Clay_ErrorHandler){HandleClayError, NULL});

  Clay_SetMeasureTextFunction(MeasureText, 0);
}

void UpdateUIInput() {
  float mouse_x;
  float mouse_y;
  SDL_MouseButtonFlags mouse_flags = SDL_GetMouseState(&mouse_x, &mouse_y);
  bool is_left_click_down = (mouse_flags & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)) != 0;

  Clay_SetPointerState((Clay_Vector2){mouse_x, mouse_y}, is_left_click_down);
}
