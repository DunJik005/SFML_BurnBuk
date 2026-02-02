cmake_minimum_required(VERSION 3.31)
project(burnbookalfa LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(burnbookalfa
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
        src/CardRender.h
        src/Tile.cpp
        src/Tile.h
        src/Textbox.cpp
        src/Textbox.h
        src/InteractionController.cpp
        src/InteractionController.h
        src/TileOverlayRenderer.cpp
        src/TileOverlayRenderer.h
        src/ReturnButton.cpp
        src/ReturnButton.h
        src/SQLite/sqlite3.c
        src/SQLite/shell.c
        src/SQLite/sqlite3.h
        src/SQLite/sqlite3ext.h)


find_package(SFML 3 COMPONENTS Graphics Window System Audio REQUIRED)

target_link_libraries(burnbookalfa
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
)