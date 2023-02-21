#include "ui_text.h"
#include <assert.h>
#include <raylib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

Vector2 textGetSize(const struct Text* text) {
  int textWidth = MeasureText(text->text, text->fontSize);
  Vector2 size = {.x = textWidth, .y = text->fontSize};
  return size;
}

char* allocateText(const char* text) {
  size_t textLen = TextLength(text) + 1;
  char* allocated = MemAlloc(textLen);
  TextCopy(allocated, text);
  return allocated;
}

size_t charsThatFitOnScreen(const char* text, int screenWidth, int fontSize) {
  size_t textLength = TextLength(text);
  for(size_t i = 1; i < textLength; ++i) {
    const char* subtextThatFits = TextSubtext(text, 0, i);
    int textSize = MeasureText(subtextThatFits, fontSize);
    if(textSize >= screenWidth) {
      return i - 1;
    }
  }
  return textLength;
}

size_t findLastWordBeforeIndex(const char* text, size_t index) {
  while(index > 0) {
    if(text[index] == ' ' || text[index] == '\n') {
      return index;
    }
    index--;
  }
  return index;
}
char* makeTextFitOnScreenWidth(const char* text, int screenWidth, int fontSize) {
  int textSizeInPixels = MeasureText(text, fontSize);
  if(screenWidth > textSizeInPixels) {
    return allocateText(text);
  }
  size_t lastCharIndexThatFits = charsThatFitOnScreen(text, screenWidth, fontSize);
  size_t lastWordBeforeIndex = findLastWordBeforeIndex(text, lastCharIndexThatFits);
  size_t textLength = TextLength(text);
  char* firstPart = allocateText(TextSubtext(text, 0, lastWordBeforeIndex));
  char* secondPart = allocateText(TextSubtext(text, lastWordBeforeIndex + 1, textLength - lastWordBeforeIndex));
  char* secondPartDone = makeTextFitOnScreenWidth(secondPart, screenWidth, fontSize);

  char* textDone = allocateText(TextFormat("%s\n%s", firstPart, secondPartDone));

  MemFree(firstPart);
  MemFree(secondPart);
  MemFree(secondPartDone);
  
  return textDone;
}

size_t getTextLastNewLinePosition(const char* text) {
  size_t textLength = TextLength(text);
  for(size_t i = textLength; i > 0; --i) {
    if(text[i] == '\n') {
      return i;
    }
  }
  return 0;
}

Vector2 getTextEndPosition(const char* text, int fontSize){
  Vector2 textArea = MeasureTextEx(GetFontDefault(), text, fontSize, fontSize / 10.0);
  size_t lastNewline = getTextLastNewLinePosition(text);
  size_t textLength = TextLength(text);
  const char* lastLine = TextSubtext(text, lastNewline + 1, textLength - (lastNewline + 1));
  int lastLineSize = MeasureText(lastLine, fontSize);
  Vector2 textEndPosition = {.x = lastLineSize, .y = textArea.y - fontSize};
  return textEndPosition;
}

Vector2 textRender(const struct Text* text, Vector2 position) {
  Vector2 screenSize = { .x = GetScreenWidth(), .y = GetScreenHeight() }; // TODO Remove this ugly hack. pass screenSize as an argument, or something
  char* textThatFitsOnScreen = makeTextFitOnScreenWidth(text->text, screenSize.x, text->fontSize);
  DrawText(textThatFitsOnScreen, position.x, position.y, text->fontSize, text->color);
  Vector2 textEndPosition = getTextEndPosition(textThatFitsOnScreen, text->fontSize);
  MemFree(textThatFitsOnScreen);
  return textEndPosition;
}

void doNothingUpdate(struct Text* _, Vector2 __) {
  
}

bool isMouseOverText(const struct Text* text, Vector2 position) {
  int textLength = MeasureText(text->text, text->fontSize);
  Rectangle textRect = {.x = position.x, .y = position.y, .width = textLength, .height = text->fontSize};
  Vector2 mousePosition = GetMousePosition();
  return CheckCollisionPointRec(mousePosition, textRect);
}
Vector2 linkRender(const struct Text* text, Vector2 position) {
  Vector2 endTextPosition = textRender(text, position);
  if(!isMouseOverText(text, position)) {
    return endTextPosition;
  }
  int textLength = MeasureText(text->text, text->fontSize);
  Vector2 startPos = {.x = position.x, .y = position.y + text->fontSize};
  Vector2 endPos = {.x = position.x + textLength, .y = position.y + text->fontSize};
  DrawLineEx(startPos, endPos, 2, text->color);
  if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { //TODO This should be in update, but too complex to fix atm
    const struct Link* link = (const struct Link*)text;
      OpenURL(link->url); 
  }
  return endTextPosition;
}
void linkUpdate(struct Text* text, Vector2 position) {
  // if(!isMouseOverText(text, position)) {
  //   return;
  // }
  // if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
  //   const struct Link* link = (const struct Link*)text;
  //   printf("trying to open url %s", link->url);
  //   OpenURL(link->url);
  // }
}


struct Text createText(const char* text, int fontSize, Color color) {
  struct Text myText = {.text = text, .fontSize = fontSize, .color = color};
  myText.render = textRender;
  myText.update = doNothingUpdate;
  return myText;
}

struct Link linkCreate(const char* text, int fontSize, Color color, const char* url) {
  struct Text linkText = createText(text, fontSize, color);
  linkText.update = linkUpdate;
  linkText.render = linkRender;
  struct Link link = {.text = linkText, .url = url};
  return link;
}
