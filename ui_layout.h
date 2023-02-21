#pragma once
#include "ui_element.h"

struct Layout {
  struct UiElement element;
  int elementCount;
};

struct Layout createLayout(Vector2 screenSize);

void layoutAddElement(struct Layout* layout, struct UiElement* element);

void measureUiElements(struct Layout* layout);

