#include "assets.h"
#include <SDL3_ttf/SDL_ttf.h>

const Color COLOR_WHITE = {255, 255, 255, 255};
const Color COLOR_BLACK = {0, 0, 0, 255};
const Color COLOR_PURE_RED = {255, 0, 0, 255};
const Color COLOR_PURE_GREEN = {0, 255, 0, 255};
const Color COLOR_PURE_BLUE = {0, 0, 255, 255};
const Color COLOR_PURE_YELLOW = {255, 255, 0, 255};
const Color COLOR_PURE_MAGENTA = {255, 0, 255, 255};
const Color COLOR_PURE_CYAN = {0, 255, 255, 255};
const Color COLOR_GRAY_50 = HEX_TO_COLOR(0xFFFFFF);
const Color COLOR_GRAY_100 = HEX_TO_COLOR(0xF4F5F5);
const Color COLOR_GRAY_200 = HEX_TO_COLOR(0xE1E3E5);
const Color COLOR_GRAY_300 = HEX_TO_COLOR(0xD5D7D9);
const Color COLOR_GRAY_400 = HEX_TO_COLOR(0xB9BDC1);
const Color COLOR_GRAY_500 = HEX_TO_COLOR(0x9A9FA4);
const Color COLOR_GRAY_600 = HEX_TO_COLOR(0x808488);
const Color COLOR_GRAY_700 = HEX_TO_COLOR(0x62676A);
const Color COLOR_GRAY_800 = HEX_TO_COLOR(0x474A4B);
const Color COLOR_GRAY_900 = HEX_TO_COLOR(0x2D3031);
const Color COLOR_GRAY_950 = HEX_TO_COLOR(0x161718);
const Color POS_ACCENT_50 = HEX_TO_COLOR(0xFEFFFF);
const Color POS_ACCENT_100 = HEX_TO_COLOR(0xD7FCF4);
const Color POS_ACCENT_200 = HEX_TO_COLOR(0x77F8E2);
const Color POS_ACCENT_300 = HEX_TO_COLOR(0x58EDD7);
const Color POS_ACCENT_400 = HEX_TO_COLOR(0x39D2BD);
const Color POS_ACCENT_500 = HEX_TO_COLOR(0x16B29F);
const Color POS_ACCENT_600 = HEX_TO_COLOR(0x119483);
const Color POS_ACCENT_700 = HEX_TO_COLOR(0x0A7366);
const Color POS_ACCENT_800 = HEX_TO_COLOR(0x05534A);
const Color POS_ACCENT_900 = HEX_TO_COLOR(0x02372F);
const Color POS_ACCENT_950 = HEX_TO_COLOR(0x011B17);
const Color NEG_ACCENT_50 = HEX_TO_COLOR(0xFFFFFF);
const Color NEG_ACCENT_100 = HEX_TO_COLOR(0xFBF2F8);
const Color NEG_ACCENT_200 = HEX_TO_COLOR(0xFADAEC);
const Color NEG_ACCENT_300 = HEX_TO_COLOR(0xF7CAE5);
const Color NEG_ACCENT_400 = HEX_TO_COLOR(0xF4A4D4);
const Color NEG_ACCENT_500 = HEX_TO_COLOR(0xEE74C3);
const Color NEG_ACCENT_600 = HEX_TO_COLOR(0xE637B3);
const Color NEG_ACCENT_700 = HEX_TO_COLOR(0xB42B8B);
const Color NEG_ACCENT_800 = HEX_TO_COLOR(0x891368);
const Color NEG_ACCENT_900 = HEX_TO_COLOR(0x600047);
const Color NEG_ACCENT_950 = HEX_TO_COLOR(0x330025);
const Color BASE_COLOR_50 = HEX_TO_COLOR(0xFFFFFF);
const Color BASE_COLOR_100 = HEX_TO_COLOR(0xFBF3F0);
const Color BASE_COLOR_200 = HEX_TO_COLOR(0xF9DDD4);
const Color BASE_COLOR_300 = HEX_TO_COLOR(0xF6CEBE);
const Color BASE_COLOR_400 = HEX_TO_COLOR(0xF0AD8B);
const Color BASE_COLOR_500 = HEX_TO_COLOR(0xDF8A52);
const Color BASE_COLOR_600 = HEX_TO_COLOR(0xBD713C);
const Color BASE_COLOR_700 = HEX_TO_COLOR(0x94572E);
const Color BASE_COLOR_800 = HEX_TO_COLOR(0x6F3D18);
const Color BASE_COLOR_900 = HEX_TO_COLOR(0x4C2606);
const Color BASE_COLOR_950 = HEX_TO_COLOR(0x281100);
const Color EVIL_COLOR_50 = HEX_TO_COLOR(0xFFFFFF);
const Color EVIL_COLOR_100 = HEX_TO_COLOR(0xFCF2F2);
const Color EVIL_COLOR_200 = HEX_TO_COLOR(0xFDDBDB);
const Color EVIL_COLOR_300 = HEX_TO_COLOR(0xFACBCC);
const Color EVIL_COLOR_400 = HEX_TO_COLOR(0xF9A7A7);
const Color EVIL_COLOR_500 = HEX_TO_COLOR(0xF6787A);
const Color EVIL_COLOR_600 = HEX_TO_COLOR(0xFA3338);
const Color EVIL_COLOR_700 = HEX_TO_COLOR(0xCA191E);
const Color EVIL_COLOR_800 = HEX_TO_COLOR(0x960A10);
const Color EVIL_COLOR_900 = HEX_TO_COLOR(0x660507);
const Color EVIL_COLOR_950 = HEX_TO_COLOR(0x370102);

TTF_Font *FONT_MAIN = NULL;
TTF_Font *FONT_DEBUG = NULL;

static TTF_Font *g_fonts = NULL;

bool AssetsInit(TTF_Font **my_fonts) {
  // This runs at runtime, so it is perfectly legal!
  FONT_MAIN = TTF_OpenFont("assets/fonts/Mansalva.ttf", 24.0F);
  if (!FONT_MAIN) {
    SDL_Log("Failed to load main font: %s", SDL_GetError());
    return false;
  }

  FONT_DEBUG = TTF_OpenFont("assets/fonts/FiraCode-Light.ttf", 24.0F);
  if (!FONT_DEBUG) {
    SDL_Log("Failed to load debug font: %s", SDL_GetError());
    return false;
  }

  *my_fonts = calloc(2, sizeof(TTF_Font *));
  g_fonts = *my_fonts;
  my_fonts[0] = FONT_DEBUG;
  my_fonts[1] = FONT_MAIN;
  return true;
}

void AssetsCleanup() {
  if (FONT_DEBUG) {
    TTF_CloseFont(FONT_DEBUG);
    FONT_DEBUG = NULL;
  }

  if (FONT_MAIN) {
    TTF_CloseFont(FONT_MAIN);
    FONT_MAIN = NULL;
  }

  free(g_fonts);
  g_fonts = NULL;
}
