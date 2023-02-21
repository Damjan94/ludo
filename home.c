#include "home.h"
#include "defines.h"
#include "piece.h"
#include "team.h"

#include <raylib.h>

void initPieceSpot(struct Square* spot, float x, float y) {
  spot->position.x = x;
  spot->position.y = y;
}

void initHomePieceSpots(struct Home* home, const Vector2 *locations) {
  for(int i = 0; i < NUMBER_OF_PIECES_PER_TEAM; ++i) {
    home->homeSquares[i].position = locations[i];
  }
}

 
void initHomeSquarePaths(struct Home* home, struct PathProgress* pathStart) {
  for(int i = 0; i < NUMBER_OF_PIECES_PER_TEAM; ++i) {
    home->homeSquarePaths[i].next = pathStart;
    home->homeSquarePaths[i].square = &(home->homeSquares[i]);
  }
}
void initHome(struct Home* home, struct PathProgress* pathStart, const Vector2 *homePositions) {
  home->numberOfPieces = NUMBER_OF_PIECES_PER_TEAM;
  home->startOfPath = pathStart;
  initHomePieceSpots(home, homePositions);
  initHomeSquarePaths(home, pathStart);
}

 bool isStartingPositionOccupied(const struct Home* home){
  return home->startOfPath->square->pieceOnThisSquare != 0;
}

const struct PathProgress* homeFindEmptySpot(const struct Home* home) {
  const struct PathProgress* freeSquare = 0;
  for(int i = 0; i < NUMBER_OF_PIECES_PER_TEAM; ++i) {
    if(isSquareEmpty(home->homeSquarePaths[i].square)) {
      freeSquare = &(home->homeSquarePaths[i]);
    }
  }
  return freeSquare;
}
void homePlacePiece(struct Home* home, struct Piece* piece) {
  struct PathProgress* freeSpotInHome =(struct PathProgress*) homeFindEmptySpot(home);
  pieceMoveToPath(piece, freeSpotInHome);
  home->numberOfPieces++;
}

void homeRemovePiece(struct Home* home, struct Piece* piece) {
  pieceMoveToPathWithCapture(piece, home->startOfPath);
  home->numberOfPieces--;
}
