#pragma once
#include <raylib.h>




struct Text {
  const char* text;
  int fontSize;
  Color color;
  Vector2(*render)(const struct Text*, Vector2);  
  void(*update)(struct Text*, Vector2);
};
struct Link {
  struct Text text;
  const char* url;
};
struct Text createText(const char* text, int fontSize, Color color);
struct Link linkCreate(const char* text, int fontSize, Color color, const char* url);
Vector2 textGetSize(const struct Text* text);
