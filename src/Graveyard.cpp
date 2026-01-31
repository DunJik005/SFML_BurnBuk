#include "Graveyard.h"
#include <iostream>
#include "NeutralTexture.h"

Graveyard::Graveyard()
    : graveyardSprite(getNeutralTexture())
{
    if (!graveyardTexture.loadFromFile("assets/tiletexture.png")) {
        std::cerr << "Ne mogu da ucitam graveyard.png\n";
    } else {
        // 🔑 resetuje textureRect (isti bug koji si imao sa deckom)
        graveyardSprite.setTexture(graveyardTexture, true);
    }
}

void Graveyard::setPosition(const Board& board, float yOffset)
{
    constexpr float TARGET_HEIGHT = 150.f;

    auto texSize = graveyardTexture.getSize();
    if (texSize.y == 0) return;

    float scale = TARGET_HEIGHT / texSize.y;
    graveyardSprite.setScale({scale, scale});

    auto bounds = graveyardSprite.getLocalBounds();
    graveyardSprite.setOrigin(
        {bounds.size.x / 2.f,
        bounds.size.y / 2.f}
    );

    // 👈 LEVO OD BOARDA (ispod decka)
    float x = board.getLeftEdge()
            - (bounds.size.x * scale) / 2.f
            - 80.f;

    float y = board.getCenterY() + yOffset;

    graveyardSprite.setPosition({x, y});

    std::cout << x << " " << y << std::endl;
}

void Graveyard::draw(sf::RenderWindow& window)
{
    window.draw(graveyardSprite);
}

bool Graveyard::contains(float x, float y) const
{
    return graveyardSprite.getGlobalBounds().contains({x, y});
}

void Graveyard::addCard(const std::shared_ptr<Card>& card)
{
    cards.push_back(card);
}

int Graveyard::getCount() const
{
    return static_cast<int>(cards.size());
}
