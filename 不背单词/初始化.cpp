#include "初始化.h"
#include "主题管理.h"
#include <SDL3_image/SDL_image.h>

int 初始化系统()
{

    if (!SDL_Init(SDL_INIT_VIDEO))
        return -1;

    if (!TTF_Init())
        return -1;

    // 创建窗口并使其居中
    window = SDL_CreateWindow("Word Book", 1920, 1080, SDL_WINDOW_RESIZABLE);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);    
    if (!window) return -1;

    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) return -1;

    SDL_Surface* role = IMG_Load("assets/role.png");
    if (role)
    {
        role_tex = SDL_CreateTextureFromSurface(renderer, role);
        SDL_DestroySurface(role);
    }

    SDL_Surface* bg = IMG_Load("assets/bg.png");
    if (!bg)
    {
        printf("背景图加载失败: %s\n", SDL_GetError());
    }
    else
    {
        background_tex = SDL_CreateTextureFromSurface(renderer, bg);
        SDL_DestroySurface(bg);
    }

    font_large = TTF_OpenFont("C:\\Windows\\Fonts\\simhei.ttf", 54);
    font_normal = TTF_OpenFont("C:\\Windows\\Fonts\\simhei.ttf", 36);
    font_small = TTF_OpenFont("C:\\Windows\\Fonts\\simhei.ttf", 28);

    if (!font_large || !font_normal || !font_small)
        return -1;

    初始化主题();
    return 0;
}


void 释放系统()
{
    if (background_tex) SDL_DestroyTexture(background_tex);
    if (role_tex) SDL_DestroyTexture(role_tex);

    if (font_large) TTF_CloseFont(font_large);
    if (font_normal) TTF_CloseFont(font_normal);
    if (font_small) TTF_CloseFont(font_small);

    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();
}