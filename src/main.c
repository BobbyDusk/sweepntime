#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include "assets.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>

const bool EXIT_ON_ESC_KEY = true;

const float PLAYER_WIDTH = 50;
const float PLAYER_HEIGHT = 50;
const float PLAYER_SPEED = 200; /* pixels per second */

typedef enum {
  DIR_UP,
  DIR_DOWN,
  DIR_LEFT,
  DIR_RIGHT,
  DIR_UP_RIGHT,
  DIR_DOWN_RIGHT,
  DIR_DOWN_LEFT,
  DIR_UP_LEFT,
  DIR_NONE
} Direction;

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  Uint64 last_frame_time;
  SDL_FPoint player_position;
  Direction player_direction;
  Direction player_horizontal_direction;
  Direction player_vertical_direction;
} AppState;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  (void)appstate;
  (void)argc;
  (void)argv;

  SDL_SetAppMetadata("Sweep'n Time", "0.1", "com.edgeofdusk.sweepntime");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  AppState *state = SDL_calloc(1, sizeof(AppState));
  if (!state) {
    SDL_Log("Couldn't allocate memory for AppState");
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("Sweep'n Time", 1920, 1080, SDL_WINDOW_RESIZABLE, &state->window,
                                   &state->renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  SDL_SetRenderLogicalPresentation(state->renderer, 1920, 1080, SDL_LOGICAL_PRESENTATION_LETTERBOX);

  state->last_frame_time = SDL_GetTicksNS();
  state->player_direction = DIR_NONE;
  state->player_horizontal_direction = DIR_NONE;
  state->player_vertical_direction = DIR_NONE;

  *appstate = state;
  return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  AppState *state = (AppState *)appstate;

  if (event->type == SDL_EVENT_KEY_DOWN) {
    switch (event->key.key) {
    case SDLK_UP:
      state->player_vertical_direction = DIR_UP;
      break;
    case SDLK_DOWN:
      state->player_vertical_direction = DIR_DOWN;
      break;
    case SDLK_LEFT:
      state->player_horizontal_direction = DIR_LEFT;
      break;
    case SDLK_RIGHT:
      state->player_horizontal_direction = DIR_RIGHT;
      break;
    default:
      break;
    }
  } else if (event->type == SDL_EVENT_KEY_UP) {
    switch (event->key.key) {
    case SDLK_UP:
      if (state->player_vertical_direction == DIR_UP) {
        state->player_vertical_direction = DIR_NONE;
      }
      break;
    case SDLK_DOWN:
      if (state->player_vertical_direction == DIR_DOWN) {
        state->player_vertical_direction = DIR_NONE;
      }
      break;
    case SDLK_LEFT:
      if (state->player_horizontal_direction == DIR_LEFT) {
        state->player_horizontal_direction = DIR_NONE;
      }
      break;
    case SDLK_RIGHT:
      if (state->player_horizontal_direction == DIR_RIGHT) {
        state->player_horizontal_direction = DIR_NONE;
      }
      break;
    default:
      break;
    }
  }

  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
  }

  if (EXIT_ON_ESC_KEY) {
    if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_ESCAPE) {
      return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
    }
  }
  return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) {
  AppState *state = (AppState *)appstate;

  Uint64 now = SDL_GetTicksNS();
  float delta_time = (now - state->last_frame_time) / 1000000000.0F;
  state->last_frame_time = now;
  switch (state->player_vertical_direction) {
  case DIR_UP:
    state->player_position.y -= PLAYER_SPEED * delta_time;
    break;
  case DIR_DOWN:
    state->player_position.y += PLAYER_SPEED * delta_time;
    break;
  default:
    break;
  }
  switch (state->player_horizontal_direction) {
  case DIR_LEFT:
    state->player_position.x -= PLAYER_SPEED * delta_time;
    break;
  case DIR_RIGHT:
    state->player_position.x += PLAYER_SPEED * delta_time;
    break;
  default:
    break;
  }

  SDL_SetRenderDrawColor(state->renderer, SDL_UNPACK_COLORS(COLOR_GRAY));

  /* clear the window to the draw color. */
  SDL_RenderClear(state->renderer);

  SDL_SetRenderDrawColor(state->renderer, SDL_UNPACK_COLORS(COLOR_GREEN));
  SDL_FRect rect = {state->player_position.x, state->player_position.y, PLAYER_WIDTH,
                    PLAYER_HEIGHT};

  SDL_RenderFillRect(state->renderer, &rect);

  /* put the newly-cleared rendering on the screen. */
  SDL_RenderPresent(state->renderer);

  return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  (void)result;
  if (appstate) {
    AppState *state = (AppState *)appstate;
    SDL_DestroyRenderer(state->renderer);
    SDL_DestroyWindow(state->window);
    SDL_free(state);
  }
  /* SDL will clean up the window/renderer for us. */
}