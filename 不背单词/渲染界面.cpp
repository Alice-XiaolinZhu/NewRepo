#include "渲染界面.h"
#include "主题管理.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

void 渲染文本(TTF_Font* font, const char* text, SDL_Color color, int x, int y, int center)
{
    if (!font || !text || !*text) return;

    SDL_Surface* surface = TTF_RenderText_Blended(font, text, (int)strlen(text), color);
    if (!surface) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        SDL_DestroySurface(surface);
        return;
    }

    SDL_FRect dst = { (float)x, (float)y, (float)surface->w, (float)surface->h };

    if (center)
        dst.x = (WINDOW_WIDTH - (float)surface->w) / 2.0f;

    SDL_RenderTexture(renderer, texture, NULL, &dst);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}

static void 渲染多行文本(TTF_Font* font, const char* text, SDL_Color color,
    int x, int y, int max_width, int line_gap)
{
    if (!font || !text || !*text) return;

    char line[1024] = { 0 };
    int line_len = 0;
    int draw_y = y;

    const char* p = text;
    while (*p)
    {
        char ch[2] = { *p, '\0' };

        char test_line[1024];
        snprintf(test_line, sizeof(test_line), "%s%s", line, ch);

        int w = 0, h = 0;
        if (!TTF_GetStringSize(font, test_line, (int)strlen(test_line), &w, &h))
        {
            if (w > max_width && line_len > 0)
            {
                渲染文本(font, line, color, x, draw_y, 0);
                draw_y += h + line_gap;

                line[0] = *p;
                line[1] = '\0';
                line_len = 1;
            }
            else
            {
                strcat_s(line, sizeof(line), ch);
                line_len++;
            }
        }

        p++;
    }

    if (line_len > 0)
    {
        渲染文本(font, line, color, x, draw_y, 0);
    }
}


void 渲染主界面()
{
    SDL_Color panel = { 18, 20, 32, 80 };
    SDL_Color panel2 = { 18, 20, 32, 80 };
    SDL_Color border = { 120, 140, 200, 120 };
    SDL_Color accent = { 255, 210, 120, 255 };

    // 增加左侧和右侧面板的大小
// 增加左侧和右侧面板的大小
    SDL_FRect left_panel = { 30.0f, 80.0f, 700.0f, 900.0f }; // 增加宽度和高度
    SDL_FRect right_panel = { 750.0f, 80.0f, 1100.0f, 900.0f }; // 调整宽度和高度    
    SDL_FRect footer_panel = { 30.0f, 1000.0f, 1820.0f, 60.0f };

    // =========================
    // 1. 背景
    // =========================
    if (background_tex)
    {
        SDL_FRect bg_dst = { 0.0f, 0.0f, (float)1920, (float)1080 };
        SDL_RenderTexture(renderer, background_tex, NULL, &bg_dst);
    }

    // =========================
    // 2. 背景暗化层
    // =========================
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
    SDL_FRect dark = { 0.0f, 0.0f, (float)1920, (float)1080 };
    SDL_RenderFillRect(renderer, &dark);

    // =========================
    // 3. 角色立绘（先画阴影，再画本体）
    // =========================
    if (role_tex)
    {
        float tex_w = 0.0f, tex_h = 0.0f;
        SDL_GetTextureSize(role_tex, &tex_w, &tex_h);

        // 你可以调这里控制人物大小
        float role_h =  1080 * 1.4f;
        float role_w = tex_w * (role_h / tex_h);

        SDL_FRect dst = {
            WINDOW_WIDTH - role_w + 1000.0f,
            WINDOW_HEIGHT - role_h + 520.0f,
            role_w,
            role_h
        };

        SDL_SetTextureColorMod(role_tex, 0, 0, 0);
        SDL_SetTextureAlphaMod(role_tex, 90);

        SDL_FRect shadow = {
            dst.x + 14.0f,
            dst.y + 18.0f,
            dst.w,
            dst.h
        };
        SDL_RenderTexture(renderer, role_tex, NULL, &shadow);

        SDL_SetTextureColorMod(role_tex, 255, 255, 255);
        SDL_SetTextureAlphaMod(role_tex, 255);
        SDL_RenderTexture(renderer, role_tex, NULL, &dst);
    }

    // =========================
    // 4. UI 面板
    // =========================
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(renderer, panel.r, panel.g, panel.b, panel.a);
    SDL_RenderFillRect(renderer, &left_panel);
    SDL_RenderFillRect(renderer, &right_panel);

    SDL_SetRenderDrawColor(renderer, panel2.r, panel2.g, panel2.b, panel2.a);
    SDL_RenderFillRect(renderer, &footer_panel);

    SDL_SetRenderDrawColor(renderer, border.r, border.g, border.b, border.a);
    SDL_RenderRect(renderer, &left_panel);
    SDL_RenderRect(renderer, &right_panel);
    SDL_RenderRect(renderer, &footer_panel);

    // =========================
    // 5. 顶部信息
    // =========================
    渲染文本(font_large, u8"单词列表", color_white, 285, 0, 0);

    char stat_line[128];
    snprintf(stat_line, sizeof(stat_line), "words: %d   selected: %d",
        word_count,
        (word_count > 0 ? selected_index + 1 : 0));
    渲染文本(font_small, stat_line, color_gray, 38, 45, 0);

    char search_line[180];
    snprintf(search_line, sizeof(search_line), "Search(S): %s%s",
        search_text,
        search_mode ? "_" : "");
    渲染文本(font_small, search_line, color_gray, 780, 40, 0);

    // =========================
    // 6. 底部提示栏
    // =========================
    渲染文本(
        font_small,
        u8"A 添加   Delete 删除   F 收藏   ↑↓ 选择   S 搜索   Tab 主题   T 测试   R 复习",
        color_white,500,1020,0
    );

    // =========================
    // 7. 空词库提示
    // =========================
    if (word_count <= 0)
    {
        渲染文本(font_normal, u8"当前词库为空", color_white, 110, 140, 0);
        渲染文本(font_small, u8"请先导入词库或按 A 添加单词", color_gray, 110, 185, 0);
        return;
    }

    Word* w = &word_list[selected_index];

    // =========================
    // 8. 左侧单词列表
    // =========================
    int start_y = 105;
    int row_h = 100;
    int visible_index = 0;

    for (int i = 0; i < word_count; i++)
    {
        if (search_text[0] != '\0')
        {
            if (strstr(word_list[i].word, search_text) == NULL &&
                strstr(word_list[i].meaning, search_text) == NULL)
            {
                continue;
            }
        }

        if (visible_index < list_scroll_offset)
        {
            visible_index++;
            continue;
        }

        int y = start_y + (visible_index - list_scroll_offset) * row_h;
        if (y > 580) break;

        SDL_FRect row = { 80.0f, (float)y +30.0f, 600.0f, 80.0f };

        SDL_SetRenderDrawColor(renderer, border.r, border.g, border.b, border.a);
        SDL_RenderRect(renderer, &row);

        if (i == selected_index)
        {
            //光标
            SDL_FRect mark = { 72.0f, (float) y+30.0f, 8.0f, 80.0f };
            SDL_SetRenderDrawColor(renderer, accent.r, accent.g, accent.b, 255);
            SDL_RenderFillRect(renderer, &mark);

            SDL_SetRenderDrawColor(renderer, 255, 180, 220, 150);
            SDL_FRect highlight = { 80.0f, (float)y + 30.0f, 600.0f, 80.0f };
            SDL_RenderFillRect(renderer, &highlight);
        }

        SDL_Color word_col = (i == selected_index) ? color_highlight : color_white;
        SDL_Color mean_col = color_gray;

        char left_word[128];
        snprintf(left_word, sizeof(left_word), "%s%s",
            word_list[i].starred ? "* " : "",
            word_list[i].word);
        //单词位置
        渲染文本(font_normal, left_word, word_col, 150, y+50, 0);
        渲染文本(font_small, word_list[i].meaning, mean_col, 400, y+50 , 0);

        visible_index++;
    }

    // =========================
    // 9. 右侧详情
    // =========================
    渲染文本(font_large, w->word, color_white, 780, 110, 0);

    char fam[64];
    snprintf(fam, sizeof(fam), "熟悉度: %d    复习次数: %d",
        w->familiarity,
        w->review_count);
    渲染文本(font_small, fam, color_gray, 780, 180, 0);

    渲染文本(font_small, u8"释义", accent, 780, 220, 0);
    渲染文本(font_normal, w->meaning, color_white, 775, 265, 0);

    渲染文本(font_small, u8"例句", accent, 780, 400, 0);
    渲染多行文本(font_small,
        w->example[0] ? w->example : u8"暂无例句",
        color_normal,
        780, 430, 430, 8);

    渲染文本(font_small, u8"词根 / 记忆提示", accent, 780, 600, 0);
    渲染多行文本(font_small,
        w->root_affix[0] ? w->root_affix : u8"暂无提示",
        color_normal,
        780, 460, 430, 8);
}

void 渲染添加界面()
{
    SDL_Color panel = current_theme.panel_color;
    SDL_Color border = current_theme.border_color;
    SDL_Color accent = current_theme.accent_color;

    SDL_FRect box = { 48.0f, 40.0f, 1830.0f, 1000.0f };

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, panel.r, panel.g, panel.b, panel.a);
    SDL_RenderFillRect(renderer, &box);

    SDL_SetRenderDrawColor(renderer, border.r, border.g, border.b, border.a);
    SDL_RenderRect(renderer, &box);

    渲染文本(font_large, u8"添加新单词", color_white, 0, 80, 1);
    渲染文本(font_small, u8"Enter 下一项 / 保存    Esc 取消    Backspace 删除字符", color_gray, 0, 200, 1);

    const char* labels[4] = {
        u8"英文单词",
        u8"中文释义",
        u8"例句",
        u8"词根 / 记忆提示"
    };

    char* values[4] = {
        input.buf_word,
        input.buf_mean,
        input.buf_example,
        input.buf_root
    };

    InputField fields[4] = {
        INPUT_WORD,
        INPUT_MEANING,
        INPUT_EXAMPLE,
        INPUT_ROOT
    };

    int start_y = 400;

    for (int i = 0; i < 4; i++)
    {
        int y = start_y + i * 140;
        SDL_Color title_col = (input.field == fields[i]) ? accent : color_white;
        SDL_Color text_col = (input.field == fields[i]) ? color_highlight : color_normal;

        SDL_FRect row = { 140.0f, (float)y - 45.0f, 1600.0f, 100.0f };

        SDL_SetRenderDrawColor(renderer, border.r, border.g, border.b, border.a);
        SDL_RenderRect(renderer, &row);

        if (input.field == fields[i])
        {
            SDL_FRect mark = { 150.0f, (float)y - 10.0f, 4.0f, 55.0f };
            SDL_SetRenderDrawColor(renderer, accent.r, accent.g, accent.b, 255);
            SDL_RenderFillRect(renderer, &mark);
        }

        渲染文本(font_small, labels[i], title_col, 160, y - 35, 0);

        char line[512];
        snprintf(line, sizeof(line), "%s%s",
            values[i],
            (input.field == fields[i]) ? "_" : "");

        渲染文本(font_normal, line[0] ? line : " ", text_col, 185, y, 0);
    }
}

void 渲染测试界面()
{
    SDL_Color accent = current_theme.accent_color;

    渲染文本(font_large, u8"测试模式", color_white, 0, 40, 1);

    if (word_count <= 0)
    {
        渲染文本(font_normal, u8"当前没有单词可测试", color_white, 0, 200, 1);
        return;
    }

    Word* w = &word_list[selected_index];
    渲染文本(font_large, w->word, color_white, 0, 180, 1);

    if (reveal_answer)
        渲染多行文本(font_normal, w->meaning, accent, 260, 270, 380, 10);
    else
        渲染文本(font_small, u8"按空格显示释义", color_gray, 0, 260, 1);

    渲染文本(font_small, u8"Esc 返回主界面", color_white, 0, 620, 1);
}

void 渲染复习界面()
{ 
    SDL_Color accent = current_theme.accent_color;

    渲染文本(font_large, u8"复习模式", color_white, 0, 40, 1);

    if (word_count <= 0)
    {
        渲染文本(font_normal, u8"当前没有单词可复习", color_white, 0, 200, 1);
        return;
    }

    Word* w = &word_list[selected_index];
    渲染文本(font_large, w->word, color_white, 0, 150, 1);

    if (reveal_answer)
    {
        渲染多行文本(font_normal, w->meaning, accent, 240, 230, 420, 10);
        渲染多行文本(font_small,
            w->example[0] ? w->example : u8"暂无例句",
            color_gray,
            220, 320, 460, 8);
    }
    else
    {
        渲染文本(font_small, u8"按空格显示释义", color_gray, 100, 230, 1);
    }

    渲染文本(font_small, u8"1 不认识   2 模糊   3 认识   Esc 返回", color_white, 0, 620, 1);
}