#include "utils.h"

bool CheckCollision(const Position *a_pos, const Size *a_size, const Position *b_pos,
                    const Size *b_size) {
  return (a_pos->x < b_pos->x + b_size->w) && (a_pos->x + a_size->w > b_pos->x) &&
         (a_pos->y < b_pos->y + b_size->h) && (a_pos->y + a_size->h > b_pos->y);
}