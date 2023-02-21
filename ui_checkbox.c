#include "defines.h"
#include "ui_checkbox.h"
#include <raylib.h>
#include <string.h>

struct CheckBox checkBoxCreate(struct Text text, bool isChecked) {
  struct CheckBox checkbox;
  memset(&checkbox, 0, sizeof(checkbox));
  checkbox.isChecked = isChecked;
  checkbox.text = text;
  checkbox.element.renderUiElement = checkBoxRender;
  checkbox.element.updateUiElement = checkBoxUpdate;

  Vector2 textSize = textGetSize(&(checkbox.text));

  Vector2 size = {.x = textSize.x + checkbox.text.fontSize, .y = textSize.y};
  uiElementSetSize(&(checkbox.element), size);
  return checkbox;
}

void checkBoxUpdate(struct UiElement* element) {
  struct CheckBox* checkbox = (struct CheckBox*) element;
  Vector2 position = uiElementGetAbsolutePosition(element);
  checkbox->text.update(&(checkbox->text), position);

  if(uiElementIsMouseOver(element)) {
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      checkbox->isChecked = !checkbox->isChecked;
    }
  }
}

Rectangle getCheckBoxOutline(const struct CheckBox* checkbox) {
  Rectangle rect;
  Vector2 checkboxPosition = uiElementGetAbsolutePosition(&(checkbox->element));
  rect.x = checkboxPosition.x;
  rect.y = checkboxPosition.y;
  rect.height = checkbox->text.fontSize;
  rect.width = checkbox->text.fontSize;
  return rect;
}

#define INDENT 3
Rectangle getCheckBoxCheckedRectangle(const Rectangle* checkboxOutline, float fontSize) {
  Rectangle checked = *checkboxOutline;
  checked.x = checkboxOutline->x + (INDENT * LINE_SIZE);
  checked.y = checkboxOutline->y + (INDENT * LINE_SIZE);
  checked.width = fontSize - (2 * INDENT * LINE_SIZE);
  checked.height = fontSize - (2 * INDENT * LINE_SIZE);
  return checked;
}

void checkBoxRender(const struct UiElement* element) {
  struct CheckBox* checkbox = (struct CheckBox*) element;
  Rectangle checkboxOutline = getCheckBoxOutline(checkbox);
  DrawRectangleLinesEx(checkboxOutline, LINE_SIZE, BLACK);
  float checkboxTextOffset = checkboxOutline.x + checkbox->text.fontSize;
  Vector2 position = {.x = checkboxTextOffset, .y = element->offset.y};
  checkbox->text.render(&(checkbox->text), position);
  Rectangle checked = getCheckBoxCheckedRectangle(&checkboxOutline, checkbox->text.fontSize - 1);
  if(checkbox->isChecked) {
    DrawRectangleRec(checked, BLACK);
  }
  if(uiElementIsMouseOver(element)) {
    DrawRectangleRec(checked, ColorAlpha(GRAY, 0.6));
  }
}

