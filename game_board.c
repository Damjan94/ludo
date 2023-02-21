#include "game_board.h"
#include "defines.h"
#include "square.h"
#include <raylib.h>
#include "path.h"
#include <stddef.h>
#include <string.h>

static bool shouldSkipSide[NUMBER_OF_TEAMS];

void initGameBoard(
  struct GameBoard* gameBoard,
  getFirstSquareIndexFn getFirstSquare,
  getFirstGoalSquareIndexFn getFirstGoalSquare,
  getPieceTextureFn getTexture,
  getHomeSquarePositionFn getHomeSquarePositions,
  getMutableSquareByIndexFn getMutableSquareByIndex,
  goHomeFn goHome)
{
  memset(shouldSkipSide, 0, sizeof(shouldSkipSide));
  initSquares(gameBoard->squares);

  initTeamsSides(gameBoard->teams);  
  for(int i = 0; i < NUMBER_OF_TEAMS; ++i) {
    initTeam(&(gameBoard->teams[i]), gameBoard->squares, getFirstSquare(i), getFirstGoalSquare(i), getTexture(i), getHomeSquarePositions, getMutableSquareByIndex, goHome);
    struct PathProgress* startOfPath = gameBoard->teams[i].home.startOfPath;
    startOfPath->square->safeFor = i;
    struct PathProgress* starPath = getNextPathMutable(startOfPath, 8);
    starPath->square->safeFor = i;
    gameBoard->teams[i].starSquare = starPath;
 }  
}

void useOwnDice(struct GameBoard* gameBoard, bool useOwnDice) {
  gameBoard->usingOwnDice = useOwnDice;
}
void goToNextState(struct GameBoard* gameBoard, enum GameState nextState) {
  gameBoard->currentState = nextState;
}
bool canPlayerRollAgain(const struct GameBoard* gameBoard) {
  return gameBoard->diceRoll == 6;
}


void movePiece(struct Piece* piece, int diceRoll) {
  struct PathProgress* nextPath = getNextPathMutable(piece->path, diceRoll);
  pieceMoveToPathWithCapture(piece, nextPath);
}


void setSkipSide(enum Side side, bool shouldSkip) {
  shouldSkipSide[side] = shouldSkip;
}


bool shouldSkipTeam(enum Side side) {
  return shouldSkipSide[side];
}

void endTurn(struct GameBoard* gameBoard) {
  do {
    gameBoard->currentPlayerTurn = (gameBoard->currentPlayerTurn + 1) % NUMBER_OF_TEAMS;
  }while(shouldSkipTeam(gameBoard->currentPlayerTurn));
}


void updateGameBoard(struct GameBoard* gameBoard, getPieceMouseClickedOnFn getClickedPiece, getDiceRollFn getDiceRoll) {
  const struct Team* currentTeam = &(gameBoard->teams[gameBoard->currentPlayerTurn]);
  switch(gameBoard->currentState) {
    case(AboutToRollDice): {
      if(IsKeyPressed(KEY_SPACE)) {
        gameBoard->diceRoll = getDiceRoll();
        goToNextState(gameBoard, RolledDice);
      }
      break;
    }
    case(RolledDice): {
      if(anyPieceHasLegalMove(currentTeam->pieces, gameBoard->diceRoll)) {
        goToNextState(gameBoard, AboutToMakeMove);
        break;
      }
      goToNextState(gameBoard, MadeMove);
      break;
    }
    case(AboutToMakeMove): {
      struct Piece* clickedPiece = getClickedPiece(currentTeam->side);
      if(!clickedPiece) {
        break;
      }
      if(!pieceHasLegalMove(clickedPiece, gameBoard->diceRoll)) {
        break;
      }
      movePiece(clickedPiece, gameBoard->diceRoll);
      goToNextState(gameBoard, MadeMove);
      break;
    }
    case(MadeMove): {
      if(canPlayerRollAgain(gameBoard)) {
        goToNextState(gameBoard, AboutToRollDice);
        break;
      }
      goToNextState(gameBoard, AwaitEndMove);
      break;
    }
    case(AwaitEndMove): {
      if(IsKeyPressed(KEY_SPACE)) {
        endTurn(gameBoard);
        if(gameBoard->usingOwnDice) {
           goToNextState(gameBoard, AboutToRollDice);
        } else {
          gameBoard->diceRoll = getDiceRoll();
          goToNextState(gameBoard, RolledDice);
        }
      }
      break;
    }
  }
}

