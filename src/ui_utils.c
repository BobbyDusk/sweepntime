#include "ui_utils.h"
#include <SDL3/SDL.h>

// A basic error handler Clay uses if you provide bad layout constraints
void HandleClayError(Clay_ErrorData errorData) {
  SDL_Log("Clay Error: %s\n", errorData.errorText.chars);
}

void InitUI(int window_width, int window_height) {
  // 1. Calculate and allocate the memory Clay needs
  uint64_t memory_size = Clay_MinMemorySize();
  Clay_Arena ui_arena = Clay_CreateArenaWithCapacityAndMemory(memory_size, malloc(memory_size));

  // 2. Initialize Clay with your window dimensions
  Clay_Initialize(ui_arena, (Clay_Dimensions){(float)window_width, (float)window_height},
                  (Clay_ErrorHandler){HandleClayError, NULL});
}

void UpdateUIInput() {
  float mouse_x;
  float mouse_y;
  // Get mouse position and button state from SDL3
  SDL_MouseButtonFlags mouse_flags = SDL_GetMouseState(&mouse_x, &mouse_y);

  bool is_left_click_down = (mouse_flags & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)) != 0;

  // Pass the state straight into Clay
  Clay_SetPointerState((Clay_Vector2){mouse_x, mouse_y}, is_left_click_down);
}

void RenderClayToSDL3(SDL_Renderer *renderer, Clay_RenderCommandArray commands) {
  for (int i = 0; i < commands.length; i++) {
    Clay_RenderCommand *cmd = &commands.internalArray[i];

    switch (cmd->commandType) {
    case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
      Clay_RectangleRenderData *config = &cmd->renderData.rectangle;

      // Set the draw color (Clay colors are 0-255 floats)
      SDL_SetRenderDrawColor(renderer, (uint8_t)config->backgroundColor.r,
                             (uint8_t)config->backgroundColor.g, (uint8_t)config->backgroundColor.b,
                             (uint8_t)config->backgroundColor.a);

      // Convert Clay bounding box to SDL_FRect (SDL3 uses floats!)
      SDL_FRect rect = {cmd->boundingBox.x, cmd->boundingBox.y, cmd->boundingBox.width,
                        cmd->boundingBox.height};
      SDL_RenderFillRect(renderer, &rect);
      break;
    }
    case CLAY_RENDER_COMMAND_TYPE_TEXT: {
      // You can map this to SDL_RenderDebugTextFormat or an SDL_TTF setup
      Clay_TextRenderData *textData = &cmd->renderData.text;
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

      // Note: Clay text strings are NOT null-terminated (they use length!)
      // We format it explicitly for SDL's debug text engine
      SDL_RenderDebugTextFormat(renderer, cmd->boundingBox.x, cmd->boundingBox.y, "%.*s",
                                textData->stringContents.length, textData->stringContents.chars);
      break;
    }
    default:
      break;
    }
  }
}