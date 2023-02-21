#include "ui_paragraph.h"
#include "raymath.h"
#include "ui_element.h"
#include "ui_text.h"
#include <raylib.h>
#include <stddef.h>
#include <string.h>

struct Paragraph {
  struct UiElement element;
  size_t count;
  struct Text* *text;
  
};
void paragraphRender(const struct UiElement* element) {
  const struct Paragraph* paragraph = (const struct Paragraph*)element;
  Vector2 endOfTextPosition = {0, 0};
  for(size_t i = 0; i < paragraph->count; ++i) {
    const struct Text* text = (paragraph->text[i]);
    Vector2 position = uiElementGetAbsolutePosition(element);
    Vector2 endPosition = text->render(text, Vector2Add(position, endOfTextPosition));
    endOfTextPosition = Vector2Add(endOfTextPosition, endPosition);
  }
}
void paragraphUpdate(struct UiElement* element) {
  struct Paragraph* paragraph = (struct Paragraph*)element;
  for(size_t i = 0; i < paragraph->count; ++i) {
    struct Text* text = (paragraph->text[i]);
    Vector2 position = uiElementGetAbsolutePosition(element);
    text->update(text, position);
  }
}


struct Paragraph* paragraphCreate() {
  struct Paragraph* paragraph = MemAlloc(sizeof(*paragraph));
  memset(paragraph, 0, sizeof(*paragraph));
  paragraph->element.renderUiElement = paragraphRender;;
  paragraph->element.updateUiElement = paragraphUpdate;
  return paragraph;
}

void paragraphDestroy(struct Paragraph* paragraph) {
  for(size_t i = 0; i < paragraph->count; ++i) { 
    MemFree(paragraph->text[i]);
    paragraph->text[i] = 0;
  }
  MemFree(paragraph->text);
  paragraph->text = 0;
  paragraph->count = 0;
  MemFree(paragraph);
}

Vector2 textGetEstimateAreaOnScreen(const struct Text* text) {
  Vector2 screen = {.x = GetScreenWidth(), .y = GetScreenHeight()};
  size_t textWidth = MeasureText(text->text, text->fontSize);
  Vector2 estimateArea = {textWidth, text->fontSize};
  if(screen.x < textWidth) {
    size_t estimateLines = textWidth / (screen.x * 0.5);
    ++estimateLines;
    estimateArea.x = screen.x;
    estimateArea.y = estimateLines * text->fontSize;
   }
  return estimateArea;
}

Vector2 paragraphGetAreaEstimate(const struct Paragraph* paragraph) {
  Vector2 estimateArea = {0, 0};
  for(size_t i = 0; i < paragraph->count; ++i) {
    const struct Text* text = paragraph->text[i];
    Vector2 textArea = textGetEstimateAreaOnScreen(text);
    if(estimateArea.x < textArea.x) {
      estimateArea.x = textArea.x;
    }
    estimateArea.y += textArea.y;
  }
  return estimateArea;
}
void paragraphAddTextToArray(struct Paragraph* paragraph, struct Text* text) {
  struct Text* *oldText = paragraph->text;
  size_t pointerSize = sizeof(struct Text*);
  paragraph->text = MemAlloc(pointerSize * (paragraph->count + 1));
  memmove(paragraph->text, oldText, paragraph->count * pointerSize);
  MemFree(oldText);
  oldText = 0;
  paragraph->text[paragraph->count++] = text;
  uiElementSetSize(&(paragraph->element), paragraphGetAreaEstimate(paragraph));
}

void paragraphAddLink(struct Paragraph* paragraph, struct Link link) {
  struct Link* newLink = MemAlloc(sizeof(link));
  *newLink = link;
  paragraphAddTextToArray(paragraph, (struct Text*)newLink);
}
void paragraphAddText(struct Paragraph* paragraph, struct Text text) {
  struct Text* newText = MemAlloc(sizeof(text));
  *newText = text;
  paragraphAddTextToArray(paragraph, newText);
}
struct UiElement* paragraphGetElement(struct Paragraph* paragraph) {
  return &(paragraph->element);
}
