#pragma once

#include <SDL3/SDL.h>
#include <clay.h>

// A basic error handler Clay uses if you provide bad layout constraints
void HandleClayError(Clay_ErrorData errorData);

void InitUI(int window_width, int window_height);

void UpdateUIInput();

void RenderClayToSDL3(SDL_Renderer *renderer, Clay_RenderCommandArray commands);