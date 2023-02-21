#include "debug.h"
#include <raylib.h>
#include <stdio.h>
#include "square.h"
#include "defines.h"
#include "utils.h"

void renderDebug(const struct Square *squares, int size) {
  for(int i = 0; i < size; ++i) {
    Vector2 position = offsetPositionBySquareSize(squares[i].position);
    DrawText(TextFormat("%d", i), position.x, position.y, 50, BLACK);  
  }
}


#include "path.h"
void printPathIndexes(const struct PathProgress *paths, int len) {
  for(int i = 0; i < len; ++i) {
    printf("%d\n", paths[i].square->index);
  }
}