#include "公共定义.h"

Word word_list[MAX_WORDS];
int word_count = 0;

InputState input = { INPUT_NONE };

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

SDL_Texture* background_tex = NULL;
SDL_Texture* role_tex = NULL;

TTF_Font* font_large = NULL;
TTF_Font* font_normal = NULL;
TTF_Font* font_small = NULL;

SDL_Color color_white = { 255, 255, 255, 255 };
SDL_Color color_highlight = { 255, 220, 100, 255 };
SDL_Color color_normal = { 220, 220, 220, 255 };
SDL_Color color_gray = { 160, 160, 160, 255 };
SDL_Color color_red = { 255, 100, 100, 255 };

int selected_index = 0;
int list_scroll_offset = 0;
PageType current_page = PAGE_MAIN;

int search_mode = 0;
char search_text[128] = { 0 };
int reveal_answer = 0;