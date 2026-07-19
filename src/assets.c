#include "assets.h"

const SDL_Color COLOR_WHITE = {255, 255, 255, SDL_ALPHA_OPAQUE};
const SDL_Color COLOR_BLACK = {0, 0, 0, SDL_ALPHA_OPAQUE};
const SDL_Color COLOR_RED = {255, 0, 0, SDL_ALPHA_OPAQUE};
const SDL_Color COLOR_GREEN = {0, 255, 0, SDL_ALPHA_OPAQUE};
const SDL_Color COLOR_BLUE = {0, 0, 255, SDL_ALPHA_OPAQUE};
const SDL_Color COLOR_YELLOW = {255, 255, 0, SDL_ALPHA_OPAQUE};
const SDL_Color COLOR_LIGHT_GRAY = {192, 192, 192, SDL_ALPHA_OPAQUE};
const SDL_Color COLOR_GRAY = {128, 128, 128, SDL_ALPHA_OPAQUE};
const SDL_Color COLOR_DARK_GRAY = {64, 64, 64, SDL_ALPHA_OPAQUE};

TTF_Font *FONT_MAIN = NULL;

bool AssetsInit() {
  // This runs at runtime, so it is perfectly legal!
  FONT_MAIN = TTF_OpenFont("assets/fonts/Mansalva.ttf", 24.0F);
  if (!FONT_MAIN) {
    SDL_Log("Failed to load main font: %s", SDL_GetError());
    return false;
  }
  return true;
}

void AssetsCleanup() {
  if (FONT_MAIN) {
    TTF_CloseFont(FONT_MAIN);
    FONT_MAIN = NULL;
  }
}