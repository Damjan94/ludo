#include "path.h"
#include "defines.h"
#include "piece.h"
#include "square.h"

void initPathProgressWithCost(struct PathProgress* progress, const struct Square* square, const struct PathProgress* next, getMutableSquareByIndexFn getMutableSquareByIndex, int cost) {
  progress->next = (struct PathProgress*)next;
  struct Square* mutableSquare = getMutableSquareByIndex(square->index);
  progress->square = mutableSquare;
  progress->cost = cost;
}

void initPathProgress(struct PathProgress* progress, const struct Square* square, const struct PathProgress* next, getMutableSquareByIndexFn getMutableSquareByIndex) {
  initPathProgressWithCost(progress, square, next, getMutableSquareByIndex, 1);
}


void initFirstPathProgress(struct PathProgress* progress, const struct Square* square, const struct PathProgress* next, getMutableSquareByIndexFn getMutableSquareByIndex) {
  initPathProgressWithCost(progress, square, next, getMutableSquareByIndex, 6);
}
void initLastPathProgress(struct PathProgress* progress, const struct Square* square, getMutableSquareByIndexFn getMutableSquareByIndex) {
  initPathProgress(progress, square, 0, getMutableSquareByIndex);
}


void initCommonPathProgress(struct PathProgress *path, const struct Square *squares, int startingSquareIndex, getMutableSquareByIndexFn getMutableSquareByIndex) {
  initFirstPathProgress(&(path[0]), getSquareByIndexWrap(squares, startingSquareIndex), &(path[1]), getMutableSquareByIndex);
  int i = 1;
  for(; i < ACCASSIBLE_SQUARE_COUNT; ++i) {
    const struct Square* square = getSquareByIndexWrap(squares, startingSquareIndex + i);
    initPathProgress(&(path[i]), square, &(path[i + 1]), getMutableSquareByIndex);
  }
}

void initGoalPathProgress(struct PathProgress *path, const struct Square *squares, int startingSquareIndex, int startingPathIndex, getMutableSquareByIndexFn getMutableSquareByIndex) {
  int i = 0;
  for(; i < GOAL_SQUARE_COUNT - 1; ++i) {
    const struct Square* square = getSquareByIndex(squares, startingSquareIndex + i);
    int pathIndex = startingPathIndex + i;
    initPathProgress(&(path[pathIndex]), square, &(path[pathIndex + 1]), getMutableSquareByIndex);
  }
  int pathIndex = startingPathIndex + i;
  initLastPathProgress(&(path[pathIndex]), getSquareByIndex(squares, startingSquareIndex + i), getMutableSquareByIndex);
}

void initPath(struct Path* path, const struct Square *squares, int firstSquareIndex, int firstGoalSquareIndex, getMutableSquareByIndexFn getMutableSquareByIndex) {
  initCommonPathProgress(path->progress , squares, firstSquareIndex, getMutableSquareByIndex);
  initGoalPathProgress(path->progress, squares, firstGoalSquareIndex, ACCASSIBLE_SQUARE_COUNT, getMutableSquareByIndex);
  //init first path
  path->first = &(path->progress[0]);
  path->last = (&path->progress[PATH_COUNT - 1]);
}


struct PathProgress* getNextPathMutable(struct PathProgress* currentPosition, int next) {
  return (struct PathProgress*)getNextPath(currentPosition, next);
}

bool hasEnoughEnergy(const struct PathProgress* path, int accumulatedCost, int energy) {
  return energy >= path->next->cost + accumulatedCost;
}

bool isPieceGoingPastLastSquare(int accumulatedEnergy, int energy) {
  return energy > accumulatedEnergy;
}

const struct PathProgress* getNextPath(const struct PathProgress* currentPosition, int energy) {
  int accumulatedCost = 0;
  const struct PathProgress* chosenPath = currentPosition;
  while(chosenPath->next && hasEnoughEnergy(chosenPath, accumulatedCost, energy)) {
    chosenPath = chosenPath->next;
    accumulatedCost += chosenPath->cost;
  }
  if(isPieceGoingPastLastSquare(accumulatedCost, energy)) { 
    chosenPath = currentPosition; 
  }
  return chosenPath;
}


bool isPositionOccupied(const struct PathProgress* path){
  return path->square->pieceOnThisSquare != 0;

}

bool isPositionOccupiedByAllies(const struct PathProgress* path, enum Side allySide) {
  const struct Piece* pieceOnPath = path->square->pieceOnThisSquare;
  if(!pieceOnPath) {
    return false;
  }
  return pieceOnPath->side == allySide;
}


