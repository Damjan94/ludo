#include "ui_button.h"
#include <raylib.h>
#include <string.h>

#define BUTTON_COLOR ORANGE
#define BUTTON_MOUSE_HOVER_COLOR RED

struct Button buttonCreate(struct Text text, void(*onClick)(void)) {
  struct Button button;
  memset(&button, 0, sizeof(button));
  button.background = BUTTON_COLOR;
  button.text = text;
  button.onClick = onClick;

  button.element.renderUiElement = buttonRender;
  button.element.updateUiElement = buttonUpdate;


  Vector2 textSize = textGetSize(&(button.text));
  uiElementSetSize(&(button.element), textSize);
  return button; 
}

void buttonRender(const struct UiElement* element) {
  struct Button* button = (struct Button*) element;
  Vector2 position = uiElementGetAbsolutePosition(element);
  Rectangle rec = {.x = position.x, .y = position.y, element->size.x, element->size.y};
  DrawRectangleRec(rec, button->background);

  button->text.render(&(button->text), position);
}

void buttonUpdate(struct UiElement* element) {
  struct Button* button = (struct Button*) element;
  Vector2 position = uiElementGetAbsolutePosition(element);
  button->text.update(&(button->text), position);
  if(uiElementIsMouseOver(element)) {
    button->background = BUTTON_COLOR;
  } else {
    button->background = BUTTON_MOUSE_HOVER_COLOR;
  }

  if(uiElementIsMouseOver(element) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    button->onClick();
  }
}



