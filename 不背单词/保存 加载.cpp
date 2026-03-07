#include <stdio.h>   // fopen, fprintf, fclose
#include "common.h"  // Word, word_list, word_count
#define _CRT_SECURE_NO_WARNINGS




void save_all_words(const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        printf("无法打开文件 %s\n", filename);
        return;
    }

    for (int i = 0; i < word_count; i++) {
        Word* w = &word_list[i];
        fprintf(fp, "%s|%s|%s|%s\n",
            w->word, w->meaning, w->example, w->root_affix);
    }
    fclose(fp);
    printf("保存成功：%d 个单词已写入 %s\n", word_count, filename);
}

void load_words(const char* path) {
    FILE* f = fopen(path, "r");
    if (!f) return;
    char line[1024];
    while (fgets(line, sizeof(line), f) && word_count < 5000) {
        // strtok 分割 |
        char* tok = strtok(line, "|");
        if (!tok) continue;
        strncpy(word_list[word_count].word, tok, MAX_WORD_LEN - 1);

        tok = strtok(NULL, "|"); if (tok) strncpy(word_list[word_count].meaning, tok, MAX_MEAN_LEN - 1);
        tok = strtok(NULL, "|"); if (tok) strncpy(word_list[word_count].example, tok, MAX_EXAMPLE_LEN - 1);
        tok = strtok(NULL, "\n"); if (tok) strncpy(word_list[word_count].root_affix, tok, MAX_ROOT_LEN - 1);

        word_count++;
    }


    fclose(f);
}