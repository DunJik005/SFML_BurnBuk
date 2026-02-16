#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Board.h"
#include "Card.h"
#include "CardHand.h"
#include "Hand.h"
#include "Deck.h"
#include "Graveyard.h"
#include "Player.h"
#include "CardView.h"
#include "TurnButtons.h"
#include "GameController.h"
#include "InteractionController.h"
#include "CardDataBase.h"
#include "MusicManager.h"
#include "PlayerView.h"

#include <ctime>
#include <cstdlib>
enum class MenuState { Main, PlayOptions, LocalOptions };

bool menu() {
    sf::RenderWindow menuWindow(sf::VideoMode({ 400, 300 }), "Main Menu");
    MenuState currentState = MenuState::Main;

    // 1. Teksture
    sf::Texture playTex, settingsTex, exitTex, backgroundTex, hostTex, joinTex, localTex, swiftTex, casualTex, backTex;
    playTex.loadFromFile("assets/menu/play.png");
    settingsTex.loadFromFile("assets/menu/options.png");
    exitTex.loadFromFile("assets/menu/exit.png");
    backgroundTex.loadFromFile("assets/backgroundtexture.png");
    hostTex.loadFromFile("assets/menu/host.png");
    joinTex.loadFromFile("assets/menu/join.png");
    localTex.loadFromFile("assets/menu/local.png");
    swiftTex.loadFromFile("assets/menu/swift.png");
    casualTex.loadFromFile("assets/menu/casual.png");
    backTex.loadFromFile("assets/menu/back.png");

    // 2. Sprite-ovi
    sf::Sprite playBtn(playTex), settingsBtn(settingsTex), exitBtn(exitTex), background(backgroundTex);
    sf::Sprite hostBtn(hostTex), joinBtn(joinTex), localBtn(localTex);
    sf::Sprite swiftBtn(swiftTex), casualBtn(casualTex);
    sf::Sprite backBtn(backTex);

    auto centerSprite = [](sf::Sprite& s) {
        auto b = s.getLocalBounds();
        s.setOrigin({ b.size.x / 2.0f, b.size.y / 2.0f });
    };

    centerSprite(playBtn); centerSprite(settingsBtn); centerSprite(exitBtn);
    centerSprite(hostBtn); centerSprite(joinBtn); centerSprite(localBtn);
    centerSprite(swiftBtn); centerSprite(casualBtn);
    centerSprite(backBtn);

    while (menuWindow.isOpen()) {
        sf::Vector2f winSize = static_cast<sf::Vector2f>(menuWindow.getSize());
        sf::Vector2f mouseWorldPos = menuWindow.mapPixelToCoords(sf::Mouse::getPosition(menuWindow));

        while (auto event = menuWindow.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                menuWindow.close();
                return false;
            }

            if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                sf::FloatRect visibleArea({ 0.f, 0.f }, { static_cast<float>(resized->size.x), static_cast<float>(resized->size.y) });
                menuWindow.setView(sf::View(visibleArea));
            }

            // ESC taster logika
            if (event->is<sf::Event::KeyPressed>()) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                    if (currentState == MenuState::PlayOptions) currentState = MenuState::Main;
                    else if (currentState == MenuState::LocalOptions) currentState = MenuState::PlayOptions;
                }
            }

            if (event->is<sf::Event::MouseButtonPressed>() && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {

                // Logika za BACK dugme (donji levi ugao)
                if (currentState != MenuState::Main) {
                    if (backBtn.getGlobalBounds().contains(mouseWorldPos)) {
                        if (currentState == MenuState::PlayOptions) currentState = MenuState::Main;
                        else if (currentState == MenuState::LocalOptions) currentState = MenuState::PlayOptions;
                    }
                }

                if (currentState == MenuState::Main) {
                    if (playBtn.getGlobalBounds().contains(mouseWorldPos)) currentState = MenuState::PlayOptions;
                    else if (exitBtn.getGlobalBounds().contains(mouseWorldPos)) { menuWindow.close(); return false; }
                }
                else if (currentState == MenuState::PlayOptions) {
                    if (localBtn.getGlobalBounds().contains(mouseWorldPos)) currentState = MenuState::LocalOptions;
                }
                else if (currentState == MenuState::LocalOptions) {
                    if (casualBtn.getGlobalBounds().contains(mouseWorldPos)) {
                        menuWindow.close();
                        return true;
                    }
                }
            }
        }

        // --- Logika pozicioniranja ---
        background.setScale({ winSize.x / background.getLocalBounds().size.x, winSize.y / background.getLocalBounds().size.y });
        float centerX = winSize.x / 2.0f;
        float centerY = winSize.y / 2.0f;
        float spacing = winSize.y * 0.22f;

        auto setupBtn = [&](sf::Sprite& s, float xPos, float yPos, float widthPercent) {
            s.setPosition({ xPos, yPos });
            float factor = (winSize.x * widthPercent) / s.getLocalBounds().size.x;
            s.setScale({ factor, factor });
            s.setColor(s.getGlobalBounds().contains(mouseWorldPos) ? sf::Color::White : sf::Color(180, 180, 180));
        };

        menuWindow.clear();
        menuWindow.draw(background);

        if (currentState == MenuState::Main) {
            setupBtn(playBtn, centerX, centerY - spacing, 0.35f);
            setupBtn(settingsBtn, centerX, centerY, 0.35f);
            setupBtn(exitBtn, centerX, centerY + spacing, 0.35f);
            menuWindow.draw(playBtn); menuWindow.draw(settingsBtn); menuWindow.draw(exitBtn);
        }
        else {
            // BACK DUGME (Sada u donjem levom uglu: 10% širine, 90% visine)
            setupBtn(backBtn, winSize.x * 0.1f, winSize.y * 0.9f, 0.15f);
            menuWindow.draw(backBtn);

            if (currentState == MenuState::PlayOptions) {
                setupBtn(hostBtn, centerX, centerY - spacing, 0.35f);
                setupBtn(joinBtn, centerX, centerY, 0.35f);
                setupBtn(localBtn, centerX, centerY + spacing, 0.35f);
                menuWindow.draw(hostBtn); menuWindow.draw(joinBtn); menuWindow.draw(localBtn);
            }
            else if (currentState == MenuState::LocalOptions) {
                setupBtn(swiftBtn, centerX, centerY - (spacing / 2.0f), 0.35f);
                setupBtn(casualBtn, centerX, centerY + (spacing / 2.0f), 0.35f);
                menuWindow.draw(swiftBtn); menuWindow.draw(casualBtn);
            }
        }

        menuWindow.display();
    }
    return false;
}

int main() {
    sf::Clock clock;
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Muzika
    MusicManager::instance().loadBackgroundPlaylist("assets/music/background");
    MusicManager::instance().playBackground();

    if (!menu()) return 0;

    // Prozori
    sf::RenderWindow window1(sf::VideoMode({1200, 800}), "Player 1");
    sf::RenderWindow window2(sf::VideoMode({1200, 800}), "Player 2");
    window1.setFramerateLimit(60);
    window2.setFramerateLimit(60);

    // Baza i deck
    CardDataBase cardDB;
    Deck gameDeck(cardDB);

    // Igrači
    Player player1;
    Player player2;

    // Board i dugmad
    Board board(1200, 800, player1, player2);
    TurnButtons turnButtons(1200, 800);

    // Game controller
    GameController gameController(board, turnButtons, gameDeck, player1, player2);
    gameDeck.setPosition(board, -80.f);

    // Ruke
    Hand p1Hand, p2Hand;
    p1Hand.onResize(1200, 800);
    p2Hand.onResize(1200, 800);

    // Graveyard
    Graveyard graveyard;
    graveyard.setPosition(board, 80);

    // CardView
    CardView cardView;
    cardView.onResize(1200, 800);

    // Interaction
    InteractionController interaction;

    // === PLAYER VIEW-ovi ===
    PlayerView view1(
        window1, Owner::Player1,
        board, p1Hand, p2Hand,
        gameDeck, graveyard,
        turnButtons, gameController,
        interaction, cardView,
        player1, player2
    );

    PlayerView view2(
        window2, Owner::Player2,
        board, p2Hand, p1Hand,
        gameDeck, graveyard,
        turnButtons, gameController,
        interaction, cardView,
        player1, player2
    );

    // === LOOP ===
    while (window1.isOpen() && window2.isOpen()) {
        float dt = clock.restart().asSeconds();

        gameController.updateHandsState(p1Hand, p2Hand);
        MusicManager::instance().update();

        // EVENTI
        while (auto e = window1.pollEvent())
            view1.handleEvent(*e);

        while (auto e = window2.pollEvent())
            view2.handleEvent(*e);

        // UPDATE
        view1.update(dt);
        view2.update(dt);

        // DRAW
        window1.clear(sf::Color::White);
        view1.draw();
        window1.display();

        window2.clear(sf::Color::White);
        view2.draw();
        window2.display();
    }
    return 0;
}
