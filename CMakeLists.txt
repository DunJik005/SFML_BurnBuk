cmake_minimum_required(VERSION 3.31)
project(sfmlproject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(sfmlproject
        src/main.cpp
        src/Card.cpp
        src/Board.cpp
        src/UI.cpp
        src/utils.cpp
        src/AssetsManager.cpp
        src/AssetsManager.h
        src/CardDataBase.cpp
        src/CardDataBase.h
        src/CardView.cpp
        src/CardView.h
        src/RarityFrame.cpp
        src/RarityFrame.h
        src/CardHand.cpp
        src/CardHand.h
        src/CardBoard.cpp
        src/CardBoard.h
        src/Hand.cpp
        src/Hand.h
        src/Deck.cpp
        src/Deck.h
        src/NeutralTexture.cpp
        src/NeutralTexture.h
        src/Graveyard.cpp
        src/Graveyard.h
        src/Player.cpp
        src/Player.h
        src/OnResize.cpp
        src/OnResize.h
        src/HandleAttack.cpp
        src/HandleAttack.h
        src/AttackSystem.cpp
        src/AttackSystem.h
        src/GameController.cpp
        src/GameController.h
        src/TurnButtons.cpp
        src/TurnButtons.h
        src/CardDataBase.cpp
        src/CardDataBase.h
        src/CardRender.cpp
        src/CardRender.h)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

target_link_libraries(sfmlproject
        SFML::Graphics
        SFML::Window
        SFML::System
)