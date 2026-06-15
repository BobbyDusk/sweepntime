#include "assets.h"
#include "components.h"
#include "config.h"
#include "systems/input.h"
#include "systems/movement.h"
#include "systems/rendering.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <flecs.h>
#include <float.h>

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  Uint64 last_frame_time;
  ecs_world_t *ecs_world;
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
  state->ecs_world = ecs_init();
  ecs_set_ctx(state->ecs_world, state->renderer, NULL);

  ECS_COMPONENT(state->ecs_world, Position);
  ECS_COMPONENT(state->ecs_world, Size);
  ECS_COMPONENT(state->ecs_world, Movement);
  ECS_COMPONENT(state->ecs_world, Visualization);
  ECS_COMPONENT(state->ecs_world, Input);

  ECS_SYSTEM(state->ecs_world, InputSystem, EcsPreUpdate, Input, Movement);
  ECS_SYSTEM(state->ecs_world, MoveSystem, EcsOnUpdate, Position, Movement);
  ECS_SYSTEM(state->ecs_world, RenderSystem, EcsPostUpdate, Position, Size, Visualization);

  ecs_entity_t player = ecs_entity(state->ecs_world, {.name = "Player"});
  ecs_set(state->ecs_world, player, Position, {100, 100});
  ecs_set(state->ecs_world, player, Size, {PLAYER_WIDTH, PLAYER_HEIGHT});
  ecs_set(state->ecs_world, player, Movement, {DIR_NONE});
  ecs_set(state->ecs_world, player, Visualization, {COLOR_GREEN});
  ecs_set(state->ecs_world, player, Input, {false, false, false, false});

  *appstate = state;
  return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  AppState *state = (AppState *)appstate;

  if (event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_KEY_UP) {
    bool is_down = (event->type == SDL_EVENT_KEY_DOWN);

    // Grab a mutable pointer to the player's input component
    ecs_entity_t player = ecs_lookup(state->ecs_world, "Player");
    ECS_COMPONENT(state->ecs_world, Input);

    switch (event->key.key) {
    case SDLK_W:
    case SDLK_UP:
      ecs_set(state->ecs_world, player, Input, {.up = is_down});
      break;
    case SDLK_S:
    case SDLK_DOWN:
      ecs_set(state->ecs_world, player, Input, {.down = is_down});
      break;
    case SDLK_A:
    case SDLK_LEFT:
      ecs_set(state->ecs_world, player, Input, {.left = is_down});
      break;
    case SDLK_D:
    case SDLK_RIGHT:
      ecs_set(state->ecs_world, player, Input, {.right = is_down});
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

bool CheckCollision(const SDL_FRect *a, const SDL_FRect *b) {
  return (a->x < b->x + b->w) && (a->x + a->w > b->x) && (a->y < b->y + b->h) &&
         (a->y + a->h > b->y);
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) {
  AppState *state = (AppState *)appstate;

  Uint64 now = SDL_GetTicksNS();
  float delta_time = (now - state->last_frame_time) / 1000000000.0F;
  state->last_frame_time = now;

  SDL_SetRenderDrawColor(state->renderer, SDL_UNPACK_COLORS(COLOR_GRAY));

  /* clear the window to the draw color. */
  SDL_RenderClear(state->renderer);

  ecs_progress(state->ecs_world, delta_time);

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
  state->player_position.y += vertical_travel;

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
  state->player_position.x += horizontal_travel;

  if (CheckCollision(&state->player, &state->object)) {
    state->object.x += horizontal_travel;
    state->object.y += vertical_travel;
  }

  SDL_SetRenderDrawColor(state->renderer, SDL_UNPACK_COLORS(COLOR_GREEN));
  SDL_RenderFillRect(state->renderer, &state->player);
  SDL_SetRenderDrawColor(state->renderer, SDL_UNPACK_COLORS(COLOR_RED));
  SDL_RenderFillRect(state->renderer, &state->object);

  SDL_SetRenderDrawColor(state->renderer, SDL_UNPACK_COLORS(COLOR_WHITE));

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
    ecs_fini(state->ecs_world);
  }
  /* SDL will clean up the window/renderer for us. */
}