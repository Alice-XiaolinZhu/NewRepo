#include "公共定义.h"
#include "初始化.h"
#include "文件读写.h"
#include "输入处理.h"
#include "渲染界面.h"
#include "主题管理.h"
#include <windows.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    if (初始化系统() != 0)
    {
        printf(u8"初始化失败\n");
        return -1;
    }

    读取单词("my_words.txt");
    printf(u8"word_count = %d\n", word_count);
    printf(u8"程序启动成功\n");

    bool quit = false;
    SDL_Event event;

    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            处理输入事件(&event);

            if (event.type == SDL_EVENT_QUIT)
                quit = true;
        }

        SDL_SetRenderDrawColor(renderer,
            current_theme.bg_color.r,
            current_theme.bg_color.g,
            current_theme.bg_color.b,
            current_theme.bg_color.a);
        SDL_RenderClear(renderer);

        if (current_page == PAGE_MAIN) {
            渲染主界面();
        }
        else if (current_page == PAGE_ADD) {
            渲染添加界面();
        }
        else if (current_page == PAGE_TEST) {
            渲染测试界面();
        }
        else if (current_page == PAGE_REVIEW) {
            渲染复习界面();
        }

        SDL_RenderPresent(renderer);
    }

    释放系统();
    return 0;
}