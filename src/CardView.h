#ifndef CARDVIEW_H
#define CARDVIEW_H

#include <SFML/Graphics.hpp>
#include "Card.h"
#include "OnResize.h"

class CardView:OnResize{
private:
    const Card* card = nullptr;
    bool visible = false;

    sf::Sprite artSprite;
    sf::Sprite frameSprite;
    mutable sf::Sprite starSprite;

    sf::Text statsText;
    sf::Text descriptionText;

    float margin = 40.f;
    float scaleFactor = 1.f;
    float starsScaleFactor = 0.22f;
    sf::Vector2f panelPos{0.f, 0.f};

    static sf::Texture& getStarTexture();
    static sf::Font& getFont();
    float lastWinW = 0.f;
    float lastWinH = 0.f;

public:
    CardView();

    void show(const Card& c);
    void hide();

    void layout(const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) const;

    void onResize(float winW,float winH) override;
};

#endif