#include "主题管理.h"

Theme current_theme = {
    { 26, 28, 44, 255 },
    { 36, 38, 56, 255 },
    { 42, 44, 66, 255 },
    { 80, 82, 110, 255 },
    { 255, 220, 100, 255 }
};

int current_theme_id = 0;

void 初始化主题()
{
    current_theme.bg_color = { 26, 28, 44, 255 };
    current_theme.panel_color = { 36, 38, 56, 255 };
    current_theme.panel2_color = { 42, 44, 66, 255 };
    current_theme.border_color = { 80, 82, 110, 255 };
    current_theme.accent_color = { 255, 220, 100, 255 };
}

void 切换主题()
{
    current_theme_id = (current_theme_id + 1) % 3;

    if (current_theme_id == 0) {
        current_theme.bg_color = { 26, 28, 44, 255 };
        current_theme.panel_color = { 36, 38, 56, 255 };
        current_theme.panel2_color = { 42, 44, 66, 255 };
        current_theme.border_color = { 80, 82, 110, 255 };
        current_theme.accent_color = { 255, 220, 100, 255 };
    }
    else if (current_theme_id == 1) {
        current_theme.bg_color = { 245, 245, 240, 255 };
        current_theme.panel_color = { 232, 232, 226, 255 };
        current_theme.panel2_color = { 220, 220, 214, 255 };
        current_theme.border_color = { 160, 160, 150, 255 };
        current_theme.accent_color = { 80, 140, 220, 255 };
    }
    else {
        current_theme.bg_color = { 225, 235, 225, 255 };
        current_theme.panel_color = { 210, 222, 210, 255 };
        current_theme.panel2_color = { 198, 212, 198, 255 };
        current_theme.border_color = { 120, 145, 120, 255 };
        current_theme.accent_color = { 220, 140, 80, 255 };
    }
}