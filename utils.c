#include "utils.h"
#include "raymath.h"
#include "defines.h"


Vector2 offsetPositionBySquareSize(Vector2 position) {
  Vector2 offset = {.x = SQUARE_WIDTH, .y = SQUARE_HEIGHT};
  return Vector2Multiply(position, offset);
}
