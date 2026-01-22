#ifndef CARDRENDERER_H
#define CARDRENDERER_H

#include <SFML/Graphics.hpp>
#include "Card.h"
#include "Textbox.h"
class CardRenderer {
public:
    CardRenderer();

    void setCard(const Card& c);
    void setScale(float s);
    void setPosition(sf::Vector2f pos);

    void draw(sf::RenderTarget& target) const;
    sf::FloatRect getBounds() const { return frameSprite.getGlobalBounds(); // global bounds uključuje poziciju i transformacije
    }
    void scrollDescription(float delta);
    void update(float dt) ;

private:
    const Card* card = nullptr;
    Textbox descriptionBox;

    sf::Sprite artSprite;
    sf::Sprite frameSprite;
    mutable sf::Sprite starSprite;

    sf::Text statsText;
    sf::Text descriptionText;
    sf::Text nameText;

    float descCharSize = 80.f;
    int visibleLines = 5;
    float scrollSpeed = 20.f;

    float scale = 1.f;
    sf::Vector2f position{0.f, 0.f};
    float starsScaleFactor = 0.22f;

    static sf::Font& getFont();
    static sf::Texture& getStarTexture();
};

#endif
