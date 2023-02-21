#pragma once
#include "ui_element.h"
#include "ui_text.h"

struct Button {
  struct UiElement element;
  Color background;
  struct Text text;
  void (*onClick)(void);
};

struct Button buttonCreate(struct Text text, void(*onClick)(void));

void buttonUpdate(struct UiElement*);
void buttonRender(const struct UiElement*);
