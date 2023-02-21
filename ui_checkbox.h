#pragma once
#include "ui_element.h"
#include "ui_text.h"

#include <stdbool.h>

struct CheckBox {
  struct UiElement element;
  struct Text text;
  bool isChecked;
};

struct CheckBox checkBoxCreate(struct Text text, bool isChecked);

void checkBoxUpdate(struct UiElement*);
void checkBoxRender(const struct UiElement*);


