cmake_minimum_required(VERSION 3.31)
project(sfmlproject)

set(CMAKE_CXX_STANDARD 20)

# Dodaj prefix path gde CMake može da nađe Ogg, Vorbis, SFML itd.
list(APPEND CMAKE_PREFIX_PATH
        "C:/SFML/lib/cmake/SFML"
        "C:/SFML/lib/cmake/Ogg"
        "C:/SFML/lib/cmake/Vorbis"
)

# Tvoj izvršni fajl
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
        src/TextBox.cpp
        src/Textbox.h
        src/InteractionController.cpp
        src/InteractionController.h
        src/TileOverlayRenderer.cpp
        src/TileOverlayRenderer.h
        src/ReturnButton.cpp
        src/ReturnButton.h
        src/MusicManager.cpp
        src/MusicManager.h
)

# Koristi statičke SFML biblioteke
set(SFML_STATIC_LIBRARIES TRUE)

# Gde je SFMLConfig.cmake
set(SFML_DIR "C:/SFML/lib/cmake/SFML")

# Pronađi SFML i sve komponente
find_package(SFML 3 COMPONENTS Graphics Window System Audio REQUIRED)

# Linkuj SFML
target_link_libraries(sfmlproject
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
)