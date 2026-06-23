#include "assets.h"
#include "components.h"
#include "config.h"
#include "systems/debug.h"
#include "systems/input.h"
#include "systems/physics.h"
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

  if (!SDL_CreateWindowAndRenderer("Sweep'n Time", 0, 0, SDL_WINDOW_FULLSCREEN, &state->window,
                                   &state->renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  SDL_SetRenderLogicalPresentation(state->renderer, 1920, 1080, SDL_LOGICAL_PRESENTATION_LETTERBOX);

  if (!SDL_SetRenderVSync(state->renderer, 1)) {
    SDL_Log("Warning: Could not enable V-Sync: %s", SDL_GetError());
  }

  state->last_frame_time = SDL_GetTicksNS();
  state->ecs_world = ecs_init();
  ecs_set_ctx(state->ecs_world, state->renderer, NULL);

  ECS_COMPONENT(state->ecs_world, Position);
  ECS_COMPONENT(state->ecs_world, Size);
  ECS_COMPONENT(state->ecs_world, Visualization);
  ECS_COMPONENT(state->ecs_world, Input);
  ECS_COMPONENT(state->ecs_world, Pushable);
  ECS_COMPONENT(state->ecs_world, Velocity);
  ECS_COMPONENT(state->ecs_world, ForceAccumulator);
  ECS_COMPONENT(state->ecs_world, PhysicsBody);

  InputSystemInit(state->ecs_world);
  PhysicsSystemInit(state->ecs_world);
  RenderSystemInit(state->ecs_world);
  DebugSystemInit(state->ecs_world);

  ecs_entity_t player = ecs_entity(state->ecs_world, {.name = "Player"});
  ecs_set(state->ecs_world, player, Position, {100, 100});
  ecs_set(state->ecs_world, player, Size, {PLAYER_WIDTH, PLAYER_HEIGHT});
  ecs_set(state->ecs_world, player, Visualization, {COLOR_GREEN});
  ecs_set(state->ecs_world, player, Input, {false, false, false, false});
  ecs_set(state->ecs_world, player, Velocity, {0.0F, 0.0F});
  ecs_set(state->ecs_world, player, ForceAccumulator, {0.0F, 0.0F});
  ecs_set(state->ecs_world, player, PhysicsBody, {PLAYER_INV_MASS, PLAYER_DAMPING});

  ecs_entity_t object = ecs_entity(state->ecs_world, {.name = "Object"});
  ecs_set(state->ecs_world, object, Position, {400, 300});
  ecs_set(state->ecs_world, object, Size, {PLAYER_WIDTH / 2, PLAYER_HEIGHT / 2});
  ecs_set(state->ecs_world, object, Visualization, {COLOR_RED});
  ecs_set(state->ecs_world, object, Pushable, {10.0F});

  *appstate = state;
  return SDL_APP_CONTINUE; /* carry on with the program! */
}

void ToggleEngineVSync(SDL_Renderer *renderer) {
  int current_mode = 0;

  // Read the actual hardware/driver state safely
  if (SDL_GetRenderVSync(renderer, &current_mode)) {
    // If it's off (0), flip it to standard on (1). Otherwise, turn it off (0).
    int new_mode = (current_mode == 0) ? 1 : 0;

    if (SDL_SetRenderVSync(renderer, new_mode)) {
      SDL_Log("V-Sync successfully changed to: %d", new_mode);
    }
  }
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  AppState *state = (AppState *)appstate;

  if (event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_KEY_UP) {
    bool is_down = (event->type == SDL_EVENT_KEY_DOWN);

    // Grab a mutable pointer to the player's input component
    ecs_entity_t player = ecs_lookup(state->ecs_world, "Player");
    ECS_COMPONENT(state->ecs_world, Input);

    Input *input = ecs_get_mut(state->ecs_world, player, Input);

    switch (event->key.key) {
    case SDLK_W:
    case SDLK_UP:
      input->up = is_down;
      break;
    case SDLK_S:
    case SDLK_DOWN:
      input->down = is_down;
      break;
    case SDLK_A:
    case SDLK_LEFT:
      input->left = is_down;
      break;
    case SDLK_D:
    case SDLK_RIGHT:
      input->right = is_down;
      break;
    }

    ecs_modified(state->ecs_world, player, Input);
  }

  if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_1) {
    ToggleEngineVSync(state->renderer);
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

  /*
  if (CheckCollision(&state->player, &state->object)) {
    state->object.x += horizontal_travel;
    state->object.y += vertical_travel;
  }
  */

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