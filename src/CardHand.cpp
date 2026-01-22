#include "CardHand.h"
#include <iostream>

// ---------- Drawing ----------
void CardHand::draw(sf::RenderWindow& window)
{
    Card::draw(window);
}

// ---------- Hit testing ----------
bool CardHand::containsPoint(float x, float y) const
{
    return getBounds().contains({ x, y });
}

// ---------- Scaling ----------
void CardHand::updateScale(float windowWidth)
{
    targetWidth = windowWidth / 12.f;
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

// ---------- Card back texture ----------
sf::Texture& CardHand::getCardBackTexture()
{
    static sf::Texture back;
    static bool loaded = false;

    if (!loaded)
    {
        if (!back.loadFromFile("assets/backgroundHand.png"))
            std::cerr << "Greska pri ucitavanju backgroundHand.png\n";
        loaded = true;
    }
    return back;
}
