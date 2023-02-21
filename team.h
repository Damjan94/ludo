#pragma once

#include <raylib.h>

#include "defines.h"
#include "path.h"
#include "piece.h"
#include "home.h"

struct Team {
  enum Side side;
  struct Path path;
  struct Home home;
  struct Piece pieces[NUMBER_OF_PIECES_PER_TEAM];
  Color color;
  const struct PathProgress* starSquare;
};
void initTeamsSides(struct Team *teams);
void initTeam(struct Team* team, const struct Square *squares, int firstSquareIndex, int firstGoalIndex, const Texture2D* texture, getHomeSquarePositionFn getHomeSquarePositions, getMutableSquareByIndexFn getMutableSquares, goHomeFn goHome);

