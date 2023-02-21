#include "team.h"
#include "piece.h"
#include "home.h"
#include "path.h"
#include <raylib.h>

const struct PathProgress* getStarSquare(const struct PathProgress* first) {
  return getNextPath(first, 8); 
}

const Color teamColors[] =  {
  {230, 41, 55, 255},//RED
  {0, 228, 48, 255}, //GREEN
  {253, 249, 0, 255},//YELLOW
  {0, 121, 241, 255} //BLUE
};

void initTeamsSides(struct Team *teams) {
  teams[0].side = Red;
  teams[1].side = Green;
  teams[2].side = Yellow;
  teams[3].side = Blue;
}

void initTeam(struct Team* team, const struct Square *squares, int firstSquareIndex, int firstGoalIndex, const Texture2D* texture, getHomeSquarePositionFn getHomeSquarePositions, getMutableSquareByIndexFn getMutableSquareByIndex, goHomeFn goHome) {
  initPath(&(team->path), squares, firstSquareIndex, firstGoalIndex, getMutableSquareByIndex);
  initHome(&(team->home), team->path.first, getHomeSquarePositions(team->side)); 
  initPieces(team->pieces, texture, team->home.homeSquarePaths, goHome, team->side);
  team->color = teamColors[team->side];
}

