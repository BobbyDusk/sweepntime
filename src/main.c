#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include "assets.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <float.h>
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
  SDL_FRect player;
  Direction player_direction;
  Direction player_horizontal_direction;
  Direction player_vertical_direction;
  SDL_FRect object;
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
  state->player.x = 100;
  state->player.y = 100;
  state->player.w = PLAYER_WIDTH;
  state->player.h = PLAYER_HEIGHT;
  state->object.x = 400;
  state->object.y = 400;
  state->object.w = 35;
  state->object.h = 35;

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

#include <SDL3/SDL.h>

Direction GetCollisionDirection(const SDL_FRect *a, const SDL_FRect *b) {
  // 1. If they aren't even intersecting, skip the math entirely
  if (!SDL_HasRectIntersectionFloat(a, b)) {
    return DIR_NONE;
  }

  // 2. Calculate the center points of both rectangles
  float center_a_x = a->x + (a->w / 2.0f);
  float center_a_y = a->y + (a->h / 2.0f);
  float center_b_x = b->x + (b->w / 2.0f);
  float center_b_y = b->y + (b->h / 2.0f);

  // 3. Calculate the distance between centers
  float delta_x = center_a_x - center_b_x;
  float delta_y = center_a_y - center_b_y;

  // 4. Calculate the combined half-widths and half-heights
  float min_dist_x = (a->w / 2.0f) + (b->w / 2.0f);
  float min_dist_y = (a->h / 2.0f) + (b->h / 2.0f);

  // 5. Calculate the exact overlap depths on both axes
  float overlap_x = min_dist_x - SDL_fabsf(delta_x);
  float overlap_y = min_dist_y - SDL_fabsf(delta_y);

  // 6. Determine the collision axis based on the shallowest penetration
  if (overlap_x < overlap_y) {
    // Collision is horizontal (Left or Right)
    if (delta_x < 0.0f) {
      return DIR_LEFT; // A is to the left of B, so it hit B's left wall
    } else {
      return DIR_RIGHT; // A is to the right of B, so it hit B's right wall
    }
  } else {
    // Collision is vertical (Top or Bottom)
    if (delta_y < 0.0f) {
      return DIR_UP; // A is above B, so it hit B's ceiling
    } else {
      return DIR_DOWN; // A is below B, so it hit B's floor
    }
  }
}
/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) {
  AppState *state = (AppState *)appstate;

  Uint64 now = SDL_GetTicksNS();
  float delta_time = (now - state->last_frame_time) / 1000000000.0F;
  state->last_frame_time = now;
  float vertical_travel = 0;
  switch (state->player_vertical_direction) {
  case DIR_UP:
    vertical_travel = -PLAYER_SPEED * delta_time;
    break;
  case DIR_DOWN:
    vertical_travel = PLAYER_SPEED * delta_time;
    break;
  default:
    break;
  }
  state->player.y += vertical_travel;

  float horizontal_travel = 0;
  switch (state->player_horizontal_direction) {
  case DIR_LEFT:
    horizontal_travel = -PLAYER_SPEED * delta_time;
    break;
  case DIR_RIGHT:
    horizontal_travel = PLAYER_SPEED * delta_time;
    break;
  default:
    break;
  }
  state->player.x += horizontal_travel;

  Direction collision_direction = GetCollisionDirection(&state->player, &state->object);
  if (collision_direction != DIR_NONE) {
    state->object.x += horizontal_travel;
    state->object.y += vertical_travel;
  }

  SDL_SetRenderDrawColor(state->renderer, SDL_UNPACK_COLORS(COLOR_GRAY));

  /* clear the window to the draw color. */
  SDL_RenderClear(state->renderer);

  SDL_SetRenderDrawColor(state->renderer, SDL_UNPACK_COLORS(COLOR_GREEN));
  SDL_RenderFillRect(state->renderer, &state->player);
  SDL_SetRenderDrawColor(state->renderer, SDL_UNPACK_COLORS(COLOR_RED));
  SDL_RenderFillRect(state->renderer, &state->object);

  SDL_SetRenderDrawColor(state->renderer, SDL_UNPACK_COLORS(COLOR_WHITE));
  switch (collision_direction) {
  case DIR_UP:
    SDL_RenderDebugText(state->renderer, 10, 10, "Collision: UP");
    break;
  case DIR_DOWN:
    SDL_RenderDebugText(state->renderer, 10, 10, "Collision: DOWN");
    break;
  case DIR_LEFT:
    SDL_RenderDebugText(state->renderer, 10, 10, "Collision: LEFT");
    break;
  case DIR_RIGHT:
    SDL_RenderDebugText(state->renderer, 10, 10, "Collision: RIGHT");
    break;
  default:
    break;
  }

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