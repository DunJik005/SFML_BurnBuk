cmake_minimum_required(VERSION 3.31)
project(sfmlproject)

set(CMAKE_CXX_STANDARD 20)

add_executable(sfmlproject
        src/main.cpp
        src/card.cpp
        src/board.cpp
        src/UI.cpp
        src/utils.cpp
        src/AssetsManager.cpp
        src/AssetsManager.h
        src/CardData.cpp
        src/CardData.h
        src/CardView.cpp
        src/CardView.h
        src/RarityFrame.cpp
        src/RarityFrame.h)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/lib/cmake/SFML)

include_directories(C:/SFML)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

target_link_libraries(sfmlproject
        SFML::Graphics
        SFML::Window
        SFML::System
)