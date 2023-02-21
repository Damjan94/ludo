#include "ui_element.h"
#include <raylib.h>
#include "raymath.h"
#include "ui_layout.h"
void uiElementSetOffset(struct UiElement* element, Vector2 offset) {
  element->offset = offset;
}
void uiElementAddOffset(struct UiElement* element, Vector2 offset) {
  element->offset = Vector2Add(element->offset, offset);
}
void uiElementSetSize(struct UiElement* element, Vector2 size) {
  element->size = size;
}
bool uiElementIsMouseOver(const struct UiElement* element) {
  Vector2 mousePosition = GetMousePosition();
  Rectangle rec = {.x = element->offset.x, .y = element->offset.y, .width = element->size.x, .height = element->size.y};
  return CheckCollisionPointRec(mousePosition, rec); 
}
Vector2 uiElementGetAbsolutePosition(const struct UiElement* element) {
  Vector2 position;
  Vector2 parentPosition = {0, 0};
  if(element->parent) {
    parentPosition = element->parent->element.offset;
  }
  position.x = parentPosition.x + element->offset.x;
  position.y = parentPosition.y + element->offset.y;
  return position;
}

