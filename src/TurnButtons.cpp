#include "TurnButtons.h"
#include <iostream>

// ---------- FONT ----------
sf::Font& TurnButtons::getFont() {
    static sf::Font font;
    static bool initialized = false;

    if (!initialized) {
        if (!font.openFromFile("assets/fonts/MatrixSmallCaps.ttf")) {
            std::cerr << "Ne mogu da ucitam font za TurnButtons\n";
        }
        initialized = true;
    }
    return font;
}

// ---------- CTOR ----------
TurnButtons::TurnButtons(float windowWidth, float windowHeight) : player1Text(getFont()),
      player2Text(getFont())

{

    float btnW = windowWidth * 0.12f;
    float btnH = windowHeight * 0.06f;
    float margin = windowWidth * 0.02f;

    // PLAYER 1 BUTTON
    player1Btn.setSize({btnW, btnH});
    player1Btn.setPosition({margin, windowHeight - btnH - margin});
    player1Btn.setFillColor(sf::Color(200, 200, 200));

    //player1Text.setFont(getFont());
    player1Text.setString("END P1 TURN");
    player1Text.setCharacterSize(24);
    player1Text.setFillColor(sf::Color::Black);

    // PLAYER 2 BUTTON
    player2Btn.setSize({btnW, btnH});
    player2Btn.setPosition({windowWidth - btnW - margin, windowHeight - btnH - margin});
    player2Btn.setFillColor(sf::Color(200, 200, 200));

    //player2Text.setFont(getFont());
    player2Text.setString("END P2 TURN");
    player2Text.setCharacterSize(24);
    player2Text.setFillColor(sf::Color::Black);

    onResize(windowWidth, windowHeight);
}

// ---------- RESIZE ----------
void TurnButtons::onResize(float windowWidth, float windowHeight) {

    float btnW = windowWidth * 0.12f;
    float btnH = windowHeight * 0.06f;
    float margin = windowWidth * 0.02f;

    player1Btn.setSize({btnW, btnH});
    player1Btn.setPosition({margin, windowHeight - btnH - margin});

    player2Btn.setSize({btnW, btnH});
    player2Btn.setPosition({windowWidth - btnW - margin, windowHeight - btnH - margin});

    // centriranje teksta u dugme
    auto centerText = [](sf::Text& txt, const sf::RectangleShape& btn) {
        sf::FloatRect bounds = txt.getLocalBounds();

        txt.setOrigin({
            bounds.position.x + bounds.size.x / 2.f,
            bounds.position.y + bounds.size.y / 2.f
        });

        txt.setPosition({
            btn.getPosition().x + btn.getSize().x / 2.f,
            btn.getPosition().y + btn.getSize().y / 2.f
        });
    };


    centerText(player1Text, player1Btn);
    centerText(player2Text, player2Btn);
}

// ---------- DRAW ----------
void TurnButtons::draw(sf::RenderWindow& window) {
    window.draw(player1Btn);
    window.draw(player2Btn);
    window.draw(player1Text);
    window.draw(player2Text);
}

// ---------- CLICK ----------
bool TurnButtons::handleClick(int mouseX, int mouseY, bool player1) {
    if (player1 &&
        player1Btn.getGlobalBounds().contains({(float)mouseX, (float)mouseY}))
        return true;

    if (!player1 &&
        player2Btn.getGlobalBounds().contains({(float)mouseX, (float)mouseY}))
        return true;

    return false;
}
