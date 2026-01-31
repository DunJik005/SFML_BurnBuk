//
// Created by Lazar Bisenic on 24. 1. 2026..
//

#ifndef BURNBOOKALFA_TILEOVERLAYRENDERER_H
#define BURNBOOKALFA_TILEOVERLAYRENDERER_H
#include "SFML/Graphics.hpp"
enum class OverlayType {
    None,
    Hover,
    Return
};

class TileOverlayRenderer {
public:
    void draw(
        sf::RenderWindow& window,
        const sf::FloatRect& bounds,
        OverlayType type
    );

private:
    sf::RectangleShape rect;
};


#endif //BURNBOOKALFA_TILEOVERLAYRENDERER_H