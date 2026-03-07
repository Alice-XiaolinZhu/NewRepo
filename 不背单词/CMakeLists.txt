cmake_minimum_required(VERSION 3.10)

# 项目名称
project(MyWordBook)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)

# 设置 SDL 和 TTF 路径
include_directories（C:\Users\溯\source\repos\不背单词\SDL3_ttf-3.2.2\lib）
include_directories（C:\Users\溯\source\repos\不背单词\SDL3-3.4.2\include）
include_directories（C:\Users\溯\source\repos\不背单词\SDL3-3.4.2\lib）
include_directories（C:\Users\溯\source\repos\不背单词\SDL3_ttf-3.2.2\include）
include_directories（C:\src\vcpkg\packages\sdl2_x64-windows\lib）
include_directories（C:\src\vcpkg\packages\sdl2_x64-windows\include）

# 指定源代码文件
set(SOURCES
    main.cpp
    # 添加更多源文件
)

# 生成可执行文件
add_executable(MyWordBook ${SOURCES})

# 链接 SDL2 和 SDL2_ttf 库
target_link_libraries(MyWordBook SDL2::SDL2 SDL2_ttf::SDL2_ttf)