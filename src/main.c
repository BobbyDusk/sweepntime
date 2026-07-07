#define CLAY_IMPLEMENTATION // required for the Clay library
#include "assets.h"
#include "components.h"
#include "config.h"
#include "object.h"
#include "systems/debug.h"
#include "systems/input.h"
#include "systems/physics.h"
#include "systems/rendering.h"
#include "systems/ui.h"
#include "ui_utils.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <box2d/box2d.h>
#include <flecs.h>
#include <float.h>
#include <hsluv.h>

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  Uint64 last_frame_time;
  ecs_world_t *ecs_world;
  ecs_query_t *movable_query;
  ecs_query_t *surfaces_query;
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

  if (!SDL_CreateWindowAndRenderer("Sweep'n Time", BASE_SCREEN_WIDTH, BASE_SCREEN_HEIGHT,
                                   SDL_WINDOW_FULLSCREEN, &state->window, &state->renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  SDL_SetRenderLogicalPresentation(state->renderer, BASE_SCREEN_WIDTH, BASE_SCREEN_HEIGHT,
                                   SDL_LOGICAL_PRESENTATION_LETTERBOX);

  if (!SDL_SetRenderVSync(state->renderer, 1)) {
    SDL_Log("Warning: Could not enable V-Sync: %s", SDL_GetError());
  }

  state->last_frame_time = SDL_GetTicksNS();
  state->ecs_world = ecs_init();
  InitUI(BASE_SCREEN_WIDTH, BASE_SCREEN_HEIGHT);

  ECS_COMPONENT(state->ecs_world, RigidBody);
  ECS_COMPONENT(state->ecs_world, Visualization);
  ECS_COMPONENT(state->ecs_world, Input);
  ECS_COMPONENT(state->ecs_world, SurfaceMaterial);
  ECS_TAG(state->ecs_world, IsPlayer);

  InputSystemInit(state->ecs_world);
  PhysicsSystemInit(state->ecs_world);
  RenderSystemInit(state->ecs_world, state->renderer);
  DebugSystemInit(state->ecs_world, state->renderer);
  UISystemInit(state->ecs_world, state->renderer);

  // ecs_entity_t sticky_surface = ecs_entity(state->ecs_world, {.name = "StickySurface"});
  // ecs_set(state->ecs_world, sticky_surface, Size, {200, 200});
  // ecs_set(state->ecs_world, sticky_surface, Visualization, {COLOR_BLUE});
  // ecs_set(state->ecs_world, sticky_surface, SurfaceMaterial, {STICKY_FRICTION_COEFFICIENT});

  // ecs_entity_t slippery_surface = ecs_entity(state->ecs_world, {.name = "SlipperySurface"});
  // ecs_set(state->ecs_world, slippery_surface, Size, {200, 200});
  // ecs_set(state->ecs_world, slippery_surface, Visualization, {COLOR_YELLOW});
  // ecs_set(state->ecs_world, slippery_surface, SurfaceMaterial, {SLIPPERY_FRICTION_COEFFICIENT});

  const float WALL_THICKNESS = 0.2F; // in meters
  ObjectPhysicsParams object_physics_params_1 = DefaultObjectPhysicsParams();
  object_physics_params_1.density = 7.0F;
  CreateObject(state->ecs_world, 10, 5, 0.4F, 0.4F, true, ConvertHSLuvToRGBA(8.4, 91.8, 35.2, 255),
               &object_physics_params_1);
  ObjectPhysicsParams object_physics_params_2 = DefaultObjectPhysicsParams();
  object_physics_params_2.density = 1.0F;
  CreateObject(state->ecs_world, 10, 7, 0.4F, 0.4F, true, ConvertHSLuvToRGBA(8.4, 91.8, 58.9, 255),
               &object_physics_params_2);

  ObjectPhysicsParams wall_physics_params = DefaultObjectPhysicsParams();
  wall_physics_params.friction = 0.0F;
  ecs_entity_t top_wall =
      CreateObject(state->ecs_world, BASE_SCREEN_WIDTH / PIXELS_PER_METER / 2, WALL_THICKNESS / 2,
                   BASE_SCREEN_WIDTH / PIXELS_PER_METER, WALL_THICKNESS, false, COLOR_DARK_GRAY,
                   &wall_physics_params);
  top_wall = ecs_set_name(state->ecs_world, top_wall, "TopWall");
  ecs_entity_t bottom_wall =
      CreateObject(state->ecs_world, BASE_SCREEN_WIDTH / PIXELS_PER_METER / 2,
                   (BASE_SCREEN_HEIGHT / PIXELS_PER_METER) - (WALL_THICKNESS / 2),
                   BASE_SCREEN_WIDTH / PIXELS_PER_METER, WALL_THICKNESS, false, COLOR_DARK_GRAY,
                   &wall_physics_params);
  bottom_wall = ecs_set_name(state->ecs_world, bottom_wall, "BottomWall");
  ecs_entity_t left_wall =
      CreateObject(state->ecs_world, WALL_THICKNESS / 2, BASE_SCREEN_HEIGHT / PIXELS_PER_METER / 2,
                   WALL_THICKNESS, BASE_SCREEN_HEIGHT / PIXELS_PER_METER, false, COLOR_DARK_GRAY,
                   &wall_physics_params);
  left_wall = ecs_set_name(state->ecs_world, left_wall, "LeftWall");
  ecs_entity_t right_wall = CreateObject(
      state->ecs_world, (BASE_SCREEN_WIDTH / PIXELS_PER_METER) - (WALL_THICKNESS / 2),
      BASE_SCREEN_HEIGHT / PIXELS_PER_METER / 2, WALL_THICKNESS,
      BASE_SCREEN_HEIGHT / PIXELS_PER_METER, false, COLOR_DARK_GRAY, &wall_physics_params);
  right_wall = ecs_set_name(state->ecs_world, right_wall, "RightWall");

  ecs_entity_t player =
      CreateObject(state->ecs_world, 2, 2, PLAYER_WIDTH, PLAYER_HEIGHT, true, COLOR_GREEN, NULL);
  ecs_set_name(state->ecs_world, player, "Player");
  ecs_set(state->ecs_world, player, Input, {false, false, false, false});

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
      return SDL_APP_SUCCESS; /* end the program, reporting success to the OS.
                               */
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

  SDL_SetRenderDrawColor(state->renderer, SDL_UNPACK_COLORS(COLOR_GRAY));

  /* clear the window to the draw color. */
  SDL_RenderClear(state->renderer);

  ecs_progress(state->ecs_world, delta_time);

  /* put the newly-cleared rendering on the screen. */
  SDL_RenderPresent(state->renderer);

  return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  (void)result;
  if (appstate) {
    AppState *state = (AppState *)appstate;
    ecs_fini(state->ecs_world); // Takes care of all entities, systems and queries
    SDL_DestroyRenderer(state->renderer);
    SDL_DestroyWindow(state->window);
    SDL_free(state);
  }
  /* SDL will clean up the window/renderer for us. */
}