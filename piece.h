#pragma once
#include <raylib.h>
#include "path.h"
#include "home.h"

struct Piece {
  struct PathProgress* path;
  const Texture2D* image;
  enum Side side;
  void (*goHome)(struct Piece* piece);
};

void initPieces(struct Piece *pieces, const Texture2D *texture, const struct PathProgress *homeSquarePaths, goHomeFn goHome, enum Side side);

bool pieceBelongsToTeam(const struct Piece* piece, const struct Piece *currentTeamPieces);
bool pieceHasLegalMove(const struct Piece* piece, int dice);
bool anyPieceHasLegalMove(const struct Piece *pieces, int diceRoll);
bool isPieceInHome(const struct Piece* piece, const struct Home* home);

struct Piece* getPieceOnSquare(const struct Square* square);

void pieceMoveToPath(struct Piece* piece, struct PathProgress* firstSquare);
void pieceMoveToPathWithCapture(struct Piece* piece, struct PathProgress* path);

