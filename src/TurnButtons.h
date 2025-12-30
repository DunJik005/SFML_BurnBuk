#ifndef SFMLPROJECT_TURNBUTTONS_H
#define SFMLPROJECT_TURNBUTTONS_H

#include <SFML/Graphics.hpp>
#include "OnResize.h"



class TurnButtons {
public:
    TurnButtons(float windowWidth, float windowHeight);

    void draw(sf::RenderWindow& window);

    // player1 == true → Player1 dugme
    bool handleClick(int mouseX, int mouseY, bool player1);

    void onResize(float windowWidth, float windowHeight);

private:
    sf::RectangleShape player1Btn;
    sf::RectangleShape player2Btn;

    sf::Text player1Text;
    sf::Text player2Text;

    static sf::Font& getFont();
};

#endif


/*

class TurnButtons{
public:
    TurnButtons(float windowWidth, float windowHeight) {
        float btnW = windowWidth * 0.1f;  // 10% širine
        float btnH = windowHeight * 0.05f; // 5% visine
        float margin = windowWidth * 0.02f;

        player1Btn.setSize({btnW, btnH});
        player1Btn.setPosition({margin, windowHeight - btnH - margin});

        player2Btn.setSize({btnW, btnH});
        player2Btn.setPosition({windowWidth - btnW - margin, windowHeight - btnH - margin});
    }

    void draw(sf::RenderWindow& window) {
        window.draw(player1Btn);
        window.draw(player2Btn);
    }

    // vraća true ako je dugme kliknuto
    bool handleClick(int mouseX, int mouseY, bool player1) {
        if (player1 && player1Btn.getGlobalBounds().contains(sf::Vector2f((float)mouseX, (float)mouseY))) {
            return true;
        }
        if (!player1 && player2Btn.getGlobalBounds().contains(sf::Vector2f((float)mouseX, (float)mouseY))) {
            return true;
        }
        return false;
    }


private:
    sf::RectangleShape player1Btn;
    sf::RectangleShape player2Btn;

    sf::Text player1Text;
    sf::Text player2Text;

    static sf::Font& getFont();
};

#endif*/