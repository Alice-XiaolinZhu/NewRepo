#include "文件读写.h"

void 保存单词(const char* path)
{
    FILE* f = fopen(path, "w");
    if (!f) return;

    for (int i = 0; i < word_count; i++)
    {
        fprintf(f, "%s|%s|%s|%s|%d|%d|%d\n",
            word_list[i].word,
            word_list[i].meaning,
            word_list[i].example,
            word_list[i].root_affix,
            word_list[i].starred,
            word_list[i].familiarity,
            word_list[i].review_count);
    }

    fclose(f);
}

void 读取单词(const char* path)
{
    FILE* f = fopen(path, "r");
    if (!f) return;

    char line[2048];

    while (fgets(line, sizeof(line), f) && word_count < MAX_WORDS)
    {
        Word* w = &word_list[word_count];
        memset(w, 0, sizeof(Word));

        int n = sscanf(line,
            "%39[^|]|%119[^|]|%299[^|]|%79[^|]|%d|%d|%d",
            w->word,
            w->meaning,
            w->example,
            w->root_affix,
            &w->starred,
            &w->familiarity,
            &w->review_count);

        if (n >= 4)
        {
            if (n < 5) w->starred = 0;
            if (n < 6) w->familiarity = 0;
            if (n < 7) w->review_count = 0;
            word_count++;
        }
    }

    fclose(f);
}