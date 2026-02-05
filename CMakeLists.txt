cmake_minimum_required(VERSION 3.31)
project(burnbookalfa LANGUAGES C CXX)

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
        src/SQLite/sqlite3.h
)


find_package(SFML 3 COMPONENTS Graphics Window System Audio REQUIRED)

target_link_libraries(burnbookalfa
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
)

# Definisanje putanja radi preglednosti
set(DB_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/burnbook.db")
set(DB_DEST "${CMAKE_CURRENT_BINARY_DIR}/burnbook.db")

# Kreira poseban "target" koji se uvek izvršava
add_custom_target(update_db ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${DB_SOURCE}" "${DB_DEST}"
    COMMENT "Proveravam i osvezavam bazu podataka u build folderu..."
    VERBATIM
)

# Primorava glavni program da saceka da se update_db zavrsi
add_dependencies(${PROJECT_NAME} update_db)