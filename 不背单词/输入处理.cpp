#include "输入处理.h"
#include "文件读写.h"
#include "主题管理.h"

static int 查找下一个匹配项(int start, int direction)
{
    if (word_count <= 0) return -1;
    if (search_text[0] == '\0') return start;

    int i = start;

    for (int step = 0; step < word_count; step++)
    {
        i += direction;

        if (i < 0) i = word_count - 1;
        if (i >= word_count) i = 0;

        if (strstr(word_list[i].word, search_text) != NULL ||
            strstr(word_list[i].meaning, search_text) != NULL)
        {
            return i;
        }
    }

    return start;
}

char* 获取当前输入缓冲(InputState* s)
{
    switch (s->field)
    {
    case INPUT_WORD:    return s->buf_word;
    case INPUT_MEANING: return s->buf_mean;
    case INPUT_EXAMPLE: return s->buf_example;
    case INPUT_ROOT:    return s->buf_root;
    default:            return NULL;
    }
}

size_t 获取当前最大长度(InputField f)
{
    switch (f)
    {
    case INPUT_WORD:    return MAX_WORD_LEN - 1;
    case INPUT_MEANING: return MAX_MEAN_LEN - 1;
    case INPUT_EXAMPLE: return MAX_EXAMPLE_LEN - 1;
    case INPUT_ROOT:    return MAX_ROOT_LEN - 1;
    default:            return 0;
    }
}

void 清空输入状态()
{
    input.field = INPUT_NONE;
    memset(input.buf_word, 0, sizeof(input.buf_word));
    memset(input.buf_mean, 0, sizeof(input.buf_mean));
    memset(input.buf_example, 0, sizeof(input.buf_example));
    memset(input.buf_root, 0, sizeof(input.buf_root));
}

void 保存当前输入为单词()
{
    if (word_count >= MAX_WORDS) return;
    if (input.buf_word[0] == '\0') return;

    Word* w = &word_list[word_count];
    memset(w, 0, sizeof(Word));

    strncpy_s(w->word, input.buf_word, MAX_WORD_LEN - 1);
    strncpy_s(w->meaning, input.buf_mean, MAX_MEAN_LEN - 1);
    strncpy_s(w->example, input.buf_example, MAX_EXAMPLE_LEN - 1);
    strncpy_s(w->root_affix, input.buf_root, MAX_ROOT_LEN - 1);

    w->starred = 0;
    w->familiarity = 0;
    w->review_count = 0;

    word_count++;
    selected_index = word_count - 1;

    保存单词("my_words.txt");
}

void 处理输入事件(SDL_Event* event)
{
    if (event->type == SDL_EVENT_TEXT_INPUT)
    {
        if (search_mode)
        {
            size_t len = strlen(search_text);
            size_t addlen = strlen(event->text.text);

            if (len + addlen < sizeof(search_text) - 1)
            {
                strncat_s(search_text, sizeof(search_text), event->text.text, _TRUNCATE);
            }

            for (int i = 0; i < word_count; i++)
            {
                if (strstr(word_list[i].word, search_text) != NULL ||
                    strstr(word_list[i].meaning, search_text) != NULL)
                {
                    selected_index = i;
                    break;
                }
            }

            return;
        }

        if (current_page == PAGE_ADD && input.field != INPUT_NONE)
        {
            char* buf = 获取当前输入缓冲(&input);
            if (buf)
            {
                size_t len = strlen(buf);
                size_t maxlen = 获取当前最大长度(input.field);
                size_t addlen = strlen(event->text.text);

                if (len + addlen < maxlen)
                {
                    strncat_s(buf, maxlen + 1, event->text.text, _TRUNCATE);
                }
            }
            return;
        }

        return;
    }

    if (event->type != SDL_EVENT_KEY_DOWN)
        return;

    SDL_Keycode key = event->key.key;

    if (search_mode)
    {
        if (key == SDLK_BACKSPACE)
        {
            size_t len = strlen(search_text);
            if (len > 0)
            {
                search_text[len - 1] = '\0';

                if (search_text[0] != '\0')
                {
                    for (int i = 0; i < word_count; i++)
                    {
                        if (strstr(word_list[i].word, search_text) != NULL ||
                            strstr(word_list[i].meaning, search_text) != NULL)
                        {
                            selected_index = i;
                            break;
                        }
                    }
                }
            }
        }
        else if (key == SDLK_DOWN)
        {
            selected_index = 查找下一个匹配项(selected_index, +1);
        }
        else if (key == SDLK_UP)
        {
            selected_index = 查找下一个匹配项(selected_index, -1);
        }
        else if (key == SDLK_RETURN || key == SDLK_KP_ENTER)
        {
            search_mode = 0;
            SDL_StopTextInput(window);
        }
        else if (key == SDLK_ESCAPE)
        {
            search_mode = 0;
            memset(search_text, 0, sizeof(search_text));
            selected_index = 0;
            list_scroll_offset = 0;
            SDL_StopTextInput(window);
        }
        return;
    }

    if (current_page == PAGE_MAIN)
    {
        if (key == SDLK_S)
        {
            search_mode = 1;
            memset(search_text, 0, sizeof(search_text));
            SDL_StartTextInput(window);
        }
        else if (key == SDLK_DOWN)
        {
            if (selected_index < word_count - 1)
                selected_index++;

            if (selected_index - list_scroll_offset > 10)
                list_scroll_offset++;
        }
        else if (key == SDLK_UP)
        {
            if (selected_index > 0)
                selected_index--;

            if (selected_index < list_scroll_offset)
                list_scroll_offset--;
        }
        else if (key == SDLK_DELETE)
        {
            if (word_count > 0)
            {
                for (int i = selected_index; i < word_count - 1; i++)
                    word_list[i] = word_list[i + 1];

                word_count--;

                if (selected_index >= word_count && word_count > 0)
                    selected_index = word_count - 1;

                if (word_count == 0)
                    selected_index = 0;

                保存单词("my_words.txt");
            }
        }
        else if (key == SDLK_F)
        {
            if (word_count > 0)
            {
                word_list[selected_index].starred = !word_list[selected_index].starred;
                保存单词("my_words.txt");
            }
        }
        else if (key == SDLK_A)
        {
            清空输入状态();
            input.field = INPUT_WORD;
            current_page = PAGE_ADD;
            SDL_StartTextInput(window);
        }
        else if (key == SDLK_TAB)
        {
            切换主题();
        }
        else if (key == SDLK_T)
        {
            reveal_answer = 0;
            current_page = PAGE_TEST;
        }
        else if (key == SDLK_R)
        {
            reveal_answer = 0;
            current_page = PAGE_REVIEW;
        }
    }
    else if (current_page == PAGE_ADD)
    {
        if (key == SDLK_ESCAPE)
        {
            清空输入状态();
            current_page = PAGE_MAIN;
            SDL_StopTextInput(window);
        }
        else if (key == SDLK_BACKSPACE)
        {
            char* buf = 获取当前输入缓冲(&input);
            if (buf)
            {
                size_t len = strlen(buf);
                if (len > 0)
                    buf[len - 1] = '\0';
            }
        }
        else if (key == SDLK_RETURN || key == SDLK_KP_ENTER)
        {
            if (input.field == INPUT_WORD)
                input.field = INPUT_MEANING;
            else if (input.field == INPUT_MEANING)
                input.field = INPUT_EXAMPLE;
            else if (input.field == INPUT_EXAMPLE)
                input.field = INPUT_ROOT;
            else if (input.field == INPUT_ROOT)
            {
                保存当前输入为单词();
                清空输入状态();
                current_page = PAGE_MAIN;
                SDL_StopTextInput(window);
            }
        }
    }
    else if (current_page == PAGE_TEST)
    {
        if (key == SDLK_ESCAPE)
        {
            current_page = PAGE_MAIN;
            reveal_answer = 0;
        }
        else if (key == SDLK_SPACE)
        {
            reveal_answer = !reveal_answer;
        }
    }
    else if (current_page == PAGE_REVIEW)
    {
        if (key == SDLK_ESCAPE)
        {
            current_page = PAGE_MAIN;
            reveal_answer = 0;
        }
        else if (key == SDLK_SPACE)
        {
            reveal_answer = !reveal_answer;
        }
        else if (key == SDLK_1)
        {
            word_list[selected_index].familiarity = 0;
            word_list[selected_index].review_count++;
            保存单词("my_words.txt");
        }
        else if (key == SDLK_2)
        {
            if (word_list[selected_index].familiarity > 0)
                word_list[selected_index].familiarity--;
            word_list[selected_index].review_count++;
            保存单词("my_words.txt");
        }
        else if (key == SDLK_3)
        {
            if (word_list[selected_index].familiarity < 5)
                word_list[selected_index].familiarity++;
            word_list[selected_index].review_count++;
            保存单词("my_words.txt");
        }
    }
}