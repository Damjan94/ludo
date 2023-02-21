#include "ui_layout.h"
#include <raylib.h>
#include <string.h>

#define LAYOUT_TRANSPARENCY 0.7
#define CORNER1 ColorTint(ColorAlpha(RED, LAYOUT_TRANSPARENCY), RAYWHITE)
#define CORNER2 ColorTint(ColorAlpha(BLUE, LAYOUT_TRANSPARENCY), RAYWHITE)
#define CORNER3 ColorTint(ColorAlpha(YELLOW, LAYOUT_TRANSPARENCY), RAYWHITE)
#define CORNER4 ColorTint(ColorAlpha(GREEN, LAYOUT_TRANSPARENCY), RAYWHITE)

void layoutRender(const struct UiElement* element) {
  const struct Layout* layout = (const struct Layout*) element;
  Vector2 position = uiElementGetAbsolutePosition(element);
  Rectangle rec = {.x = position.x, .y = position.y, .width = element->size.x, .height = element->size.y};
  DrawRectangleGradientEx(rec, CORNER1, CORNER2, CORNER3, CORNER4);
  for(const struct UiElement* element = layout->element.nextElement; element != 0;  element = element->nextElement) {
    element->renderUiElement(element); 
  }
}

void layoutUpdate(struct UiElement* myElement) {
  struct Layout* layout = (struct Layout*) myElement;
  for(struct UiElement* element = layout->element.nextElement; element != 0;  element = element->nextElement) {
    element->updateUiElement(element);
  }
}

struct Layout createLayout(Vector2 screenSize) {
  struct Layout layout;
  memset(&layout, 0, sizeof(layout));
  uiElementSetSize(&(layout.element),screenSize);

  layout.element.updateUiElement = layoutUpdate;
  layout.element.renderUiElement = layoutRender;
  return layout;
}
struct UiElement* layoutGetLastElement(struct Layout* layout) {
  struct UiElement* element = layout->element.nextElement;
  if(element == 0) {
    return &(layout->element);
  }
  while(element->nextElement != 0) {
    element = element->nextElement;
  }
  return element;
}

bool layoutIsEmpty(const struct Layout* layout) {
  return layout->elementCount <= 0;
}


void layoutAddElement(struct Layout* layout, struct UiElement* element) {
  struct UiElement* lastElement = layoutGetLastElement(layout);
  Vector2 offset = {0, 0};
  if(!layoutIsEmpty(layout)) {
    offset.y += lastElement->offset.y + lastElement->size.y;
  }
  element->parent = layout;
  lastElement->nextElement = element;
  layout->elementCount++;
  uiElementSetOffset(element, offset);
}

