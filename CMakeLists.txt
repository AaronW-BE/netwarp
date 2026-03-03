cmake_minimum_required(VERSION 3.16)
project(GameAccelerator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

add_executable(GameAccelerator
    src/main.cpp
    src/MainWindow.cpp
    include/MainWindow.h
)

target_link_libraries(GameAccelerator PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# Optional: Set executable icon for Windows
# if(WIN32)
#     target_sources(GameAccelerator PRIVATE res/windows_icon.rc)
# endif()
