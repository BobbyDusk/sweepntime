#pragma once

#include <SDL3/SDL.h>
#include <flecs.h>
#include <clay_renderer.h>

void UISystemInit(ecs_world_t *world, SDL_Renderer *renderer, TTF_TextEngine *textEngine,
                  TTF_Font **myFonts);
void UISystem(ecs_iter_t *it);