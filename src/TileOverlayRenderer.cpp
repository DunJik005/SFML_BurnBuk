//
// Created by Lazar Bisenic on 24. 1. 2026..
//

#include "TileOverlayRenderer.h"


#include "TileOverlayRenderer.h"

void TileOverlayRenderer::draw(
    sf::RenderWindow& window,
    const sf::FloatRect& bounds,
    OverlayType type
)
{
    if (type == OverlayType::None)
        return;

    rect.setPosition(bounds.position);
    rect.setSize(bounds.size);

    switch (type)
    {
        case OverlayType::Hover:
            rect.setFillColor(sf::Color(255, 255, 255, 40));
            rect.setOutlineThickness(0.f);
            break;

        case OverlayType::Return:
            rect.setOutlineThickness(2.f);
            rect.setOutlineColor(sf::Color::White);
            break;

        default:
            return;
    }

    window.draw(rect);
}
