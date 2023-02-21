#pragma once

#include <raylib.h>
#include "ui_element.h"
#include "ui_text.h"

struct Paragraph;
struct Paragraph* paragraphCreate();
void paragraphDestroy(struct Paragraph* paragraph);

void paragraphAddLink(struct Paragraph* paragraph, struct Link link);
void paragraphAddText(struct Paragraph* paragraph, struct Text text);

struct UiElement* paragraphGetElement(struct Paragraph* paragraph);
