#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_LEN     40
#define MAX_MEAN_LEN     120
#define MAX_EXAMPLE_LEN  300
#define MAX_ROOT_LEN     80
#define MAX_WORDS        5000

#define WINDOW_WIDTH     900
#define WINDOW_HEIGHT    700

typedef struct {
    char word[MAX_WORD_LEN];
    char meaning[MAX_MEAN_LEN];
    char example[MAX_EXAMPLE_LEN];
    char root_affix[MAX_ROOT_LEN];

    int starred;          // 是否收藏
    int familiarity;      // 熟悉度
    int review_count;     // 复习次数
} Word;

typedef enum {
    INPUT_NONE = 0,
    INPUT_WORD,
    INPUT_MEANING,
    INPUT_EXAMPLE,
    INPUT_ROOT
} InputField;

typedef struct {
    InputField field;
    char buf_word[MAX_WORD_LEN];
    char buf_mean[MAX_MEAN_LEN];
    char buf_example[MAX_EXAMPLE_LEN];
    char buf_root[MAX_ROOT_LEN];
} InputState;

typedef enum {
    PAGE_MAIN = 0,
    PAGE_ADD,
    PAGE_TEST,
    PAGE_REVIEW
} PageType;

extern Word word_list[MAX_WORDS];
extern int word_count;

extern InputState input;

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern SDL_Texture* background_tex;
extern SDL_Texture* role_tex;

extern TTF_Font* font_large;
extern TTF_Font* font_normal;
extern TTF_Font* font_small;

extern SDL_Color color_white;
extern SDL_Color color_highlight;
extern SDL_Color color_normal;
extern SDL_Color color_gray;
extern SDL_Color color_red;

extern int selected_index;
extern int list_scroll_offset;
extern PageType current_page;

extern int search_mode;
extern char search_text[128];
extern int reveal_answer;

// 公共定义.h 里加入
extern int window_width;
extern int window_height;