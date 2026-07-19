#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <clay.h>

// A basic error handler Clay uses if you provide bad layout constraints
void HandleClayError(Clay_ErrorData errorData);

Clay_Dimensions MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData);

void InitUI(int window_width, int window_height, TTF_Font **fontsArray);

void UpdateUIInput();