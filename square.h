#pragma once
#include <raylib.h>
#include "defines.h"

struct Piece;
 struct Square {
  Vector2 position;
  int index;
  struct Piece* pieceOnThisSquare;
  enum Side safeFor;
};

void initSquares(struct Square *squares);

const struct Square* getSquareByIndexWrap(const struct Square *squares, int index); 
const struct Square* getSquareByIndex(const struct Square *squares, int index); 
bool isSquareEmpty(const struct Square* square);

