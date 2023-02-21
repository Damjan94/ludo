#pragma once

#include <stdbool.h>
#include "square.h"
#include "defines.h"
#include "path.h"

struct Home{
  int numberOfPieces;
  struct Square homeSquares[NUMBER_OF_PIECES_PER_TEAM];
  struct PathProgress homeSquarePaths[NUMBER_OF_PIECES_PER_TEAM];
  struct PathProgress* startOfPath;
};

struct Team;
// void initHomes(struct Team *teams); 
void initHome(struct Home* home, struct PathProgress* pathStart, const Vector2 *homePositions);

bool isStartingPositionOccupied(const struct Home* home);
void homePlacePiece(struct Home* home, struct Piece* piece);
void homeRemovePiece(struct Home* home, struct Piece* piece);

