#pragma once

enum GameState { STATE_MENU, STATE_PLAYING, STATE_PAUSE, STATE_GAME_OVER };

bool initializeState(enum GameState *state);
void switchState(enum GameState new_state);
void destroyState();
