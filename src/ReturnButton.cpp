#include "ReturnButton.h"

ReturnButton::ReturnButton()
{
    button.setSize({100.f, 100.f});
    button.setFillColor(sf::Color(255, 0, 0, 160));
    button.setOutlineThickness(2.f);
    button.setOutlineColor(sf::Color::White);
}

void ReturnButton::setBounds(const sf::FloatRect& cardBounds)
{
    // gore-desno na karti
    const float buttonHeight = cardBounds.size.y / 3.f;

    button.setPosition({
        cardBounds.position.x,
        cardBounds.position.y + cardBounds.size.y - buttonHeight
    });

    button.setSize({
        cardBounds.size.x,
        buttonHeight
    });
}

void ReturnButton::setVisible(bool v)
{
    visible = v;
}

void ReturnButton::draw(sf::RenderWindow& window)
{
    if (visible)
        window.draw(button);
}

bool ReturnButton::contains(float x, float y) const
{
    if (!visible) return false;
    return button.getGlobalBounds().contains({x, y});
}
