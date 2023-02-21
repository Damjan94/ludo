#include "piece.h"
#include "game_board.h"
#include "home.h"
#include "path.h"
#include "team.h"

void initPiece(struct Piece* piece, const Texture2D* texture, const struct PathProgress* homePath, goHomeFn goHome, enum Side side) {
  piece->path = (struct PathProgress*)homePath;
  homePath->square->pieceOnThisSquare = piece;
  piece->image = texture;
  piece->goHome = goHome;
  piece->side = side;
}

void initPieces(struct Piece *pieces, const Texture2D *texture, const struct PathProgress *homeSquarePaths, goHomeFn goHome, enum Side side) {
  for(int i = 0; i < NUMBER_OF_PIECES_PER_TEAM; ++i) {
    initPiece(&(pieces[i]), texture, &(homeSquarePaths[i]), goHome, side);
  }
}

bool isPieceInHome(const struct Piece* piece, const struct Home* home){
  for(int i = 0; i < NUMBER_OF_PIECES_PER_TEAM; ++i) {
    if(&(home->homeSquares[i]) == piece->path->square) {
      return true;
    }
  }
  return false;
}

bool isPieceOnSafeSquare(const struct Piece* piece) {
  return piece->path->square->safeFor == piece->side;
}
bool isPieceMovableToPath(const struct Piece* me, const struct PathProgress* path) {
  if(!isPositionOccupied(path)) {
    return true;
  }
  if(isPositionOccupiedByAllies(path, me->side)) {
    return false;
  }

  const struct Piece* enemy = getPieceOnSquare(path->square);
  if(isPieceOnSafeSquare(enemy)) {
    return false;
  }
  return true;
}
bool pieceHasLegalMove(const struct Piece* piece, int dice){
  const struct PathProgress* nextPath = getNextPath(piece->path, dice);
  return isPieceMovableToPath(piece, nextPath);// Conveniently, if getNextPath fails, and returns our current path, we are on this path, meaning we are allies to ourselves.
}

bool anyPieceHasLegalMove(const struct Piece *pieces, int diceRoll){
  bool legalMoveFound = false;
  for(int i = 0; i < NUMBER_OF_PIECES_PER_TEAM; ++i) {
    if(pieceHasLegalMove(&(pieces[i]), diceRoll)) {
      legalMoveFound = true;
    }
  }
  return legalMoveFound;
}
struct Piece* getPieceOnSquare(const struct Square* square) {
  return square->pieceOnThisSquare;
}
bool pieceBelongsToTeam(const struct Piece* piece, const struct Piece *currentTeamPieces) {
  for(int i = 0; i < NUMBER_OF_PIECES_PER_TEAM; ++i) {
    if(piece == &(currentTeamPieces[i])) {
      return true;
    }
  }
  return false;
}

void pieceMoveToPath(struct Piece* piece, struct PathProgress* path) {
  piece->path->square->pieceOnThisSquare = 0;
  piece->path = path;
  piece->path->square->pieceOnThisSquare = piece;
}

void pieceMoveToPathWithCapture(struct Piece* piece, struct PathProgress* path) {
  struct Piece* enemy = getPieceOnSquare(path->square);
  if(enemy) {
      enemy->goHome(enemy);
  }
  pieceMoveToPath(piece, path);
}

