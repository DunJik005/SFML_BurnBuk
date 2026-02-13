#ifndef SFMLPROJECT_CARDHAND_H
#define SFMLPROJECT_CARDHAND_H

#include "Card.h"
#include <SFML/Graphics.hpp>
#include "CardRender.h"

class CardHand : public Card {
public:
    // ---------- Ctor ----------
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
    int handIndex = -1;

    // ---------- Renderer ----------
    CardRenderer &getCardRenderer() {return renderer;}

private:
    CardRenderer renderer;
    // ciljna širina u odnosu na ekran
    float targetWidth = 0.f;
    // helper
    void applyScaleFromWidth(float width);
};

#endif // SFMLPROJECT_CARDHAND_H
