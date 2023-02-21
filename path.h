#pragma once

#include "defines.h"
#include <stdbool.h>

struct PathProgress {
  int cost;
  struct Square* square;
  struct PathProgress* next;
};

struct Path {
  struct PathProgress progress[PATH_COUNT];
  struct PathProgress* first;
  struct PathProgress* last;
};

void initPath(struct Path* path, const struct Square *squares, int firstSquareIndex, int firstGoalSquareIndex, getMutableSquareByIndexFn getMutableSquareByIndex); 

const struct PathProgress* getNextPath(const struct PathProgress* currentPosition, int next);
struct PathProgress* getNextPathMutable(struct PathProgress* currentPosition, int next);
struct Piece;
bool isPositionOccupiedByAllies(const struct PathProgress* path, enum Side allySide);
bool isPositionOccupied(const struct PathProgress* path); 