#include "square.h"
#include "path.h"
#include "game_board.h"
  
void setSquare(struct Square* square, Vector2 position, int index) {
  square->position = position;
  square->index = index;
  square->safeFor = None;

}

int goRight(int startIndex, int count, struct Vector2 startPosition, struct Square *squares) {
  int i = startIndex;
  for(; i <= startIndex + count; ++i) {
    squares[i].index = i;
    squares[i].position = startPosition;
    startPosition.x += 1;
  }
  return i;
}

int goLeft(int startIndex, int count, struct Vector2 startPosition, struct Square *squares) {
  int i = startIndex;
  for(; i <= startIndex + count; ++i) {
    squares[i].index = i;
    squares[i].position = startPosition;
    startPosition.x -= 1;
  }
  return i;
}
int goUp(int startIndex, int count, struct Vector2 startPosition, struct Square *squares) {
  int i = startIndex;
  for(; i <= startIndex + count; ++i) {
    squares[i].index = i;
    squares[i].position = startPosition;
    startPosition.y -= 1;
  }
  return i;
}
int  goDown(int startIndex, int count, struct Vector2 startPosition, struct Square *squares) {
  int i = startIndex;
  for(; i <= startIndex + count; ++i) {
    squares[i].index = i;
    squares[i].position = startPosition;
    startPosition.y += 1;
  }
  return i;
}

void setPosition(Vector2 *position, int x, int y){
  position->x = x;
  position->y = y;
}

 void initSquares(struct Square *squares) {
  int index = 0;
  Vector2 position = {.x = 1, .y = 6};
  index = goRight(index, 4, position, squares);
  
  setPosition(&position, 6, 5);
  index = goUp(index, 5, position, squares);

  setPosition(&position, 7, 0);
  index = goRight(index, 1, position, squares);

  setPosition(&position, 8, 1);
  index = goDown(index, 4, position, squares);

  setPosition(&position, 9, 6);
  index = goRight(index, 5, position, squares);

  setPosition(&position, 14, 7);
  index = goDown(index, 1, position, squares);

  setPosition(&position, 13, 8);
  index = goLeft(index, 4, position, squares);

  setPosition(&position, 8, 9);
  index = goDown(index, 5, position, squares);

  setPosition(&position, 7, 14);
  index = goLeft(index, 1, position, squares);

  setPosition(&position, 6, 13);
  index = goUp(index, 4, position, squares);

  setPosition(&position, 5, 8);
  index = goLeft(index, 5, position, squares);

  setPosition(&position, 0, 7);
  index = goUp(index, 1, position, squares);

  // goals
  setPosition(&position, 1, 7);
  index = goRight(index, 5, position, squares);
  setPosition(&position, 7, 1);
  index = goDown(index, 5, position, squares);
  setPosition(&position, 13, 7);
  index = goLeft(index, 5, position, squares);
  setPosition(&position, 7, 13);
  index = goUp(index, 5, position, squares);
 }


const struct Square* getSquareByIndexWrap(const struct Square *squares, int index) {
  return &(squares[index % SQUARE_COUNT]);
}
const struct Square* getSquareByIndex(const struct Square *squares, int index) {
  return &(squares[index]);
}

bool isSquareEmpty(const struct Square* square) {
  return square->pieceOnThisSquare == 0;
}

