#include "state.h"
#include <SDL3/SDL.h>

static enum GameState *g_game_state;

bool initializeState(enum GameState *state) {
  state = SDL_calloc(1, sizeof(enum GameState));
  if (!state) {
    SDL_Log("Couldn't allocate memory for GameState");
    return false;
  }
  *state = STATE_MENU;
  g_game_state = state;
  return true;
}

void switchState(enum GameState new_state) {
  *g_game_state = new_state;
}

void destroyState() {
  SDL_free(g_game_state);
}
