#pragma once

#include "defines.h"
#include "square.h"
#include "team.h"

enum GameState {
  AboutToRollDice,
  RolledDice,
  AboutToMakeMove,
  MadeMove,
  AwaitEndMove
};
struct GameBoard {
  enum Side currentPlayerTurn;
  enum GameState currentState;
  int diceRoll;
  bool usingOwnDice;

  struct Square squares[BOARD_SIZE];
  struct Team teams[NUMBER_OF_TEAMS];
};


void initGameBoard(
  struct GameBoard* gameBoard,
  getFirstSquareIndexFn getFirstSquare,
  getFirstGoalSquareIndexFn getFirstGoalSquare,
  getPieceTextureFn getTexture,
  getHomeSquarePositionFn getHomeSquarePositions,
  getMutableSquareByIndexFn getMutableSquareByIndex,
  goHomeFn goHome);

 
void updateGameBoard(struct GameBoard* gameBoard,getPieceMouseClickedOnFn getClickedPiece, getDiceRollFn getDiceRoll);

void useOwnDice(struct GameBoard* gameBoard, bool useOwnDice);
void setSkipSide(enum Side side, bool shouldSkip);


