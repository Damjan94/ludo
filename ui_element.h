#pragma once
#include <raylib.h>

struct UiElement {
  Vector2 offset;
  Vector2 size;
  struct UiElement* nextElement;
  struct Layout* parent;
  void (*renderUiElement)(const struct UiElement*); 
  void (*updateUiElement)(struct UiElement*);
};

void uiElementSetOffset(struct UiElement* element, Vector2 offset);
void uiElementAddOffset(struct UiElement* element, Vector2 offset);
void uiElementSetSize(struct UiElement* element, Vector2 size);

bool uiElementIsMouseOver(const struct UiElement* element);

Vector2 uiElementGetAbsolutePosition(const struct UiElement* element);
