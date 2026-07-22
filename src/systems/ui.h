#pragma once

#include <SDL3/SDL.h>
#include <clay_renderer.h>
#include <flecs.h>

void UISystemInit(ecs_world_t *world, SDL_Renderer *renderer, TTF_TextEngine *text_engine, TTF_Font **my_fonts);
void UISystem(ecs_iter_t *it);
