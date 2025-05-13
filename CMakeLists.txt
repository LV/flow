cmake_minimum_required(VERSION 4.0)
project(flow LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-std=c++2c)

add_subdirectory(third_party/ftxui)

add_executable(flow src/main.cpp)

include_directories(third_party/tomlplusplus/include)

target_link_libraries(flow PRIVATE
    ftxui::screen
    ftxui::dom
    ftxui::component
)
