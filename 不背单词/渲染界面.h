#pragma once
#include "公共定义.h"

void 渲染文本(TTF_Font* font, const char* text, SDL_Color color, int x, int y, int center);

void 渲染主界面();
void 渲染添加界面();
void 渲染测试界面();
void 渲染复习界面();