#include "CardHand.h"
#include <iostream>
#include <SFML/Graphics.hpp>
// ---------- Drawing ----------
void CardHand::draw(sf::RenderWindow& window)
{
    renderer.setCard(*this);
    sf::Sprite& s = getSprite();
    renderer.setPosition(s.getPosition());
    renderer.setScale(s.getScale().x); // uniform scale

    renderer.draw(window);
}

// ---------- Hit testing ----------
bool CardHand::containsPoint(float x, float y) const
{
    return getBounds().contains({ x, y });
}

// ---------- Scaling ----------
void CardHand::updateScale(float windowWidth)
{
    targetWidth = windowWidth / 5.f;
    applyScaleFromWidth(targetWidth);
}

void CardHand::applyScaleFromWidth(float width)
{
    sf::Sprite& s = getSprite();
    sf::FloatRect bounds = s.getLocalBounds();

    if (bounds.size.x == 0.f)
        return;

    float scale = width / bounds.size.x;
    s.setScale({scale, scale});
}

float CardHand::getWidth() const
{
    return getBounds().size.x;
}

float CardHand::getHeight() const
{
    return getBounds().size.y;
}

// ---------- Hand index ----------
void CardHand::setHandIndex(int index)
{
    handIndex = index;
}

int CardHand::getHandIndex() const
{
    return handIndex;
}
