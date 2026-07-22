#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <clay.h>

#define SDL_COLOR(c) (c).r, (c).g, (c).b, (c).a
#define CLAY_COLOR(c) ((Clay_Color){(float)(c).r, (float)(c).g, (float)(c).b, (float)(c).a})
#define HEX_TO_COLOR(hex) ((Color){.r = (float)(((hex) >> 16) & 0xFF), .g = (float)(((hex) >> 8) & 0xFF), .b = (float)((hex) & 0xFF), .a = 255.0f})

typedef struct {
  Uint8 r;
  Uint8 g;
  Uint8 b;
  Uint8 a;
} Color;

extern const Color COLOR_WHITE;
extern const Color COLOR_BLACK;
extern const Color COLOR_PURE_RED;
extern const Color COLOR_PURE_GREEN;
extern const Color COLOR_PURE_BLUE;
extern const Color COLOR_PURE_YELLOW;
extern const Color COLOR_PURE_MAGENTA;
extern const Color COLOR_PURE_CYAN;
extern const Color COLOR_GRAY_50;
extern const Color COLOR_GRAY_100;
extern const Color COLOR_GRAY_200;
extern const Color COLOR_GRAY_300;
extern const Color COLOR_GRAY_400;
extern const Color COLOR_GRAY_500;
extern const Color COLOR_GRAY_600;
extern const Color COLOR_GRAY_700;
extern const Color COLOR_GRAY_800;
extern const Color COLOR_GRAY_900;
extern const Color COLOR_GRAY_950;
extern const Color POS_ACCENT_50;
extern const Color POS_ACCENT_100;
extern const Color POS_ACCENT_200;
extern const Color POS_ACCENT_300;
extern const Color POS_ACCENT_400;
extern const Color POS_ACCENT_500;
extern const Color POS_ACCENT_600;
extern const Color POS_ACCENT_700;
extern const Color POS_ACCENT_800;
extern const Color POS_ACCENT_900;
extern const Color POS_ACCENT_950;
extern const Color NEG_ACCENT_50;
extern const Color NEG_ACCENT_100;
extern const Color NEG_ACCENT_200;
extern const Color NEG_ACCENT_300;
extern const Color NEG_ACCENT_400;
extern const Color NEG_ACCENT_500;
extern const Color NEG_ACCENT_600;
extern const Color NEG_ACCENT_700;
extern const Color NEG_ACCENT_800;
extern const Color NEG_ACCENT_900;
extern const Color NEG_ACCENT_950;
extern const Color BASE_COLOR_50;
extern const Color BASE_COLOR_100;
extern const Color BASE_COLOR_200;
extern const Color BASE_COLOR_300;
extern const Color BASE_COLOR_400;
extern const Color BASE_COLOR_500;
extern const Color BASE_COLOR_600;
extern const Color BASE_COLOR_700;
extern const Color BASE_COLOR_800;
extern const Color BASE_COLOR_900;
extern const Color BASE_COLOR_950;

extern const Color EVIL_COLOR_50;
extern const Color EVIL_COLOR_100;
extern const Color EVIL_COLOR_200;
extern const Color EVIL_COLOR_300;
extern const Color EVIL_COLOR_400;
extern const Color EVIL_COLOR_500;
extern const Color EVIL_COLOR_600;
extern const Color EVIL_COLOR_700;
extern const Color EVIL_COLOR_800;
extern const Color EVIL_COLOR_900;
extern const Color EVIL_COLOR_950;

extern TTF_Font *FONT_DEBUG;
#define FONT_DEBUG_ID 0
extern TTF_Font *FONT_MAIN;
#define FONT_MAIN_ID 1

TTF_Font **AssetsInit(void);
void AssetsCleanup();
