#pragma once
#include "公共定义.h"

char* 获取当前输入缓冲(InputState* s);
size_t 获取当前最大长度(InputField f);
void 清空输入状态();
void 保存当前输入为单词();
void 处理输入事件(SDL_Event* event);