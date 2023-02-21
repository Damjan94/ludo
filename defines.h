#pragma once
#include <stdbool.h>

#define FONT_SIZE 45
#define FONT_COLOR RAYWHITE
#define LINE_SIZE 3

#define SQUARE_WIDTH 79
#define SQUARE_HEIGHT 80
#define BOARD_SIZE 100
#define NUMBER_OF_TEAMS 4
#define NUMBER_OF_PIECES_PER_TEAM 4
#define GOAL_SQUARE_COUNT 5
#define SQUARE_COUNT 52
#define INACCESSIBLE_SQUARE_COUNT 1
#define ACCASSIBLE_SQUARE_COUNT (SQUARE_COUNT - INACCESSIBLE_SQUARE_COUNT)
#define PATH_COUNT (ACCASSIBLE_SQUARE_COUNT + GOAL_SQUARE_COUNT)

enum Side {
  Red = 0,
  Green,
  Yellow,
  Blue,
  None
};


struct Vector2;
typedef const struct Vector2* (getHomeSquarePositionFn)(enum Side);
struct Texture;
typedef const struct Texture* (getPieceTextureFn)(enum Side);
typedef int (getFirstSquareIndexFn)(enum Side);
typedef int (getFirstGoalSquareIndexFn)(enum Side);

struct Square;
typedef struct Square* (getMutableSquareByIndexFn)(int index);

struct Piece;
typedef void (goHomeFn)(struct Piece*);

typedef struct Piece* getPieceMouseClickedOnFn(enum Side side);

typedef int getDiceRollFn();

