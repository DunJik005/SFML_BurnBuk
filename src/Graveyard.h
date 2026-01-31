#ifndef SFMLPROJECT_GRAVEYARD_H
#define SFMLPROJECT_GRAVEYARD_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Card.h"
#include "Board.h"

class Graveyard {
public:
    Graveyard();

    void setPosition(const Board& board, float yOffset);
    void draw(sf::RenderWindow& window);
    bool contains(float x, float y) const;

    void addCard(const std::shared_ptr<Card>& card);
    int getCount() const;

private:
    sf::Texture graveyardTexture;
    sf::Sprite  graveyardSprite;

    std::vector<std::shared_ptr<Card>> cards;
};

#endif
