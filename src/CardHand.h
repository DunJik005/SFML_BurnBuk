#ifndef SFMLPROJECT_CARDHAND_H
#define SFMLPROJECT_CARDHAND_H

#include "Card.h"
#include <SFML/Graphics.hpp>

class CardHand : public Card {
public:
    using Card::Card;

    // ---------- Drawing ----------
    void draw(sf::RenderWindow& window) override;

    // ---------- Hit testing ----------
    bool containsPoint(float x, float y) const;

    // ---------- Size / layout ----------
    void updateScale(float windowWidth);   // poziva Hand / onResize
    float getWidth()  const;               // za sabijanje
    float getHeight() const;

    // ---------- Hand index ----------
    void setHandIndex(int index);
    int  getHandIndex() const;

    // ---------- Textures ----------
    static sf::Texture& getCardBackTexture();

    int handIndex = -1;

private:

    // ciljna širina u odnosu na ekran
    float targetWidth = 0.f;

    // helper
    void applyScaleFromWidth(float width);
};

#endif // SFMLPROJECT_CARDHAND_H
