#include <SDL3/SDL.h>
#include <stdio.h>
#include <string.h>

// 定义最大单词数和单词长度
#define MAX_WORDS 100
#define MAX_LENGTH 50

// 定义结构体存储单词和翻译
typedef struct {
    char word[MAX_LENGTH];
    char translation[MAX_LENGTH];
    char imagePath[MAX_LENGTH];  // 单词的图片路径
} Word;

// 函数声明
void addWord(Word words[], int* count);
void quiz(Word words[], int count, SDL_Renderer* renderer);
void showAllWords(Word words[], int count, SDL_Renderer* renderer);

int main() {
    // 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL 初始化失败: %s\n", SDL_GetError());
        return 1;
    }

    // 创建窗口
    //SDL_Window* window = SDL_CreateWindow("不背单词", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Window* window = SDL_CreateWindow("不背单词", 800, 600, 0);
    if (!window) {
        printf("窗口创建失败: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // 创建渲染器
    //SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);  // NULL 或 -1 让 SDL 自动选驱动，flags 0
    if (!renderer) {
        printf("渲染器创建失败: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Word words[MAX_WORDS];  // 用来存储单词和翻译
    int wordCount = 0;
    int choice;

    while (1) {
        // 显示菜单
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 白色背景
        SDL_RenderPresent(renderer);

        printf("\n=== 不背单词 ===\n");
        printf("1. 添加单词\n");
        printf("2. 开始测验\n");
        printf("3. 显示所有单词\n");
        printf("4. 退出\n");
        printf("请选择操作（1-4）：");
        scanf_s("%d", &choice);
        getchar();  // 清除换行符

        switch (choice) {
        case 1:
            addWord(words, &wordCount);  // 添加单词
            break;
        case 2:
            quiz(words, wordCount, renderer);  // 开始测验
            break;
        case 3:
            showAllWords(words, wordCount, renderer);  // 显示所有单词
            break;
        case 4:
            printf("感谢使用！再见。\n");
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 0;  // 退出程序
        default:
            printf("无效选择，请重新输入！\n");
        }
    }

    return 0;
}

// 添加单词到单词库
void addWord(Word words[], int* count) {
    if (*count >= MAX_WORDS) {
        printf("单词库已满，无法添加更多单词！\n");
        return;
    }
    printf("请输入单词：");
    fgets(words[*count].word, MAX_LENGTH, stdin);
    words[*count].word[strcspn(words[*count].word, "\n")] = 0;  // 去掉换行符

    printf("请输入翻译：");
    fgets(words[*count].translation, MAX_LENGTH, stdin);
    words[*count].translation[strcspn(words[*count].translation, "\n")] = 0;  // 去掉换行符

    printf("请输入图片路径（例如：image.jpg）：");
    fgets(words[*count].imagePath, MAX_LENGTH, stdin);
    words[*count].imagePath[strcspn(words[*count].imagePath, "\n")] = 0;  // 去掉换行符

    (*count)++;
    printf("单词添加成功！\n");
}

// 开始测验
void quiz(Word words[], int count, SDL_Renderer* renderer) {
    if (count == 0) {
        printf("没有单词可供测试，添加单词后再试！\n");
        return;
    }

    int score = 0;
    for (int i = 0; i < count; i++) {
        char answer[MAX_LENGTH];

        // 加载图片（BMP 格式，SDL_LoadBMP 没变）
        SDL_Surface* imageSurface = SDL_LoadBMP(words[i].imagePath);
        if (!imageSurface) {
            printf("加载图片失败 %s: %s\n", words[i].imagePath, SDL_GetError());
            continue;  // 跳过这张图
        }

        // 创建纹理（函数名没变，但确认返回 SDL_Texture*）
        SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
        SDL_DestroySurface(imageSurface);  // 立即销毁 surface，释放内存

        if (!imageTexture) {
            printf("创建纹理失败: %s\n", SDL_GetError());
            continue;
        }

        // 清屏（白色背景，可选，根据需求改颜色）
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // 定义源矩形（整个图片）和目标矩形（显示位置/大小）
        SDL_FRect src = { 0, 0, (float)imageSurface->w, (float)imageSurface->h };  // 源：整张图（用 SDL_FRect 为 subpixel 精度）
        SDL_FRect dst = { 100, 100, 600, 400 };  // 目标：屏幕上位置和大小（可调整）

        // 渲染纹理（SDL3 新函数，替换 SDL_RenderCopy）
        SDL_RenderTexture(renderer, imageTexture, &src, &dst);

        SDL_RenderPresent(renderer);  // 更新屏幕

        // 显示单词并等待输入（控制台）
        printf("单词: %s\n", words[i].word);
        printf("请输入翻译: ");
        fgets(answer, MAX_LENGTH, stdin);
        answer[strcspn(answer, "\n")] = 0;  // 去掉换行

        if (strcmp(answer, words[i].translation) == 0) {
            printf("回答正确！\n");
            score++;
        }
        else {
            printf("回答错误！正确翻译是: %s\n", words[i].translation);
        }

        // 清理纹理（必须销毁，否则内存泄漏）
        SDL_DestroyTexture(imageTexture);

        // 可选：加延时或按键继续
        SDL_Delay(1000);  // 暂停 1 秒看图
    }

    printf("\n测试结束！你得了 %d/%d 分。\n", score, count);
}

// 显示所有单词和翻译
void showAllWords(Word words[], int count, SDL_Renderer* renderer) {
    if (count == 0) {
        printf("没有单词！\n");
        return;
    }

    printf("\n所有单词：\n");
    for (int i = 0; i < count; i++) {
        printf("单词：%s, 翻译：%s\n", words[i].word, words[i].translation);
    }
}
