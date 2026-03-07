#pragma once
#include "公共定义.h"

typedef struct {
    SDL_Color bg_color;
    SDL_Color panel_color;
    SDL_Color panel2_color;
    SDL_Color border_color;
    SDL_Color accent_color;
} Theme;

extern Theme current_theme;
extern int current_theme_id;


void 初始化主题();
void 切换主题();