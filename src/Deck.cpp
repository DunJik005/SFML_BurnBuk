#include "Deck.h"
#include <random>
#include <iostream>
#include "NeutralTexture.h"

Deck::Deck(CardDataBase& db)
    : database(db), deckSprite(getNeutralTexture())
{
    auto allCards = db.getAllCards();

    // Dodajemo sve karte iz baze u deck (jedinstvene instance)
    for (auto& c : allCards) {
        cards.push_back(c);
    }

    if (!deckTexture.loadFromFile("assets/tiletexture.png")) {
        std::cerr << "Ne mogu da ucitam deckic.png\n";
    } else {
        deckSprite.setTexture(deckTexture, true);
    }

    std::cout << "Deck ima " << cards.size() << " karata.\n";
}

void Deck::setPosition(const Board& board, float yOffset)
{
    constexpr float TARGET_HEIGHT = 150.f;

    auto texSize = deckTexture.getSize();
    if (texSize.y == 0) return;

    float scale = TARGET_HEIGHT / texSize.y;
    deckSprite.setScale({scale, scale});

    auto bounds = deckSprite.getLocalBounds();
    deckSprite.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    float x = board.getLeftEdge() - deckSprite.getGlobalBounds().size.x / 2.f - 80.f;
    float y = board.getCenterY() + yOffset ;
    deckSprite.setPosition({x, y});
}


void Deck::draw(sf::RenderWindow& window) {
    window.draw(deckSprite);
}

bool Deck::contains(float x, float y) {
    return deckSprite.getGlobalBounds().contains({x, y});
}

bool Deck::drawCardToHand(Hand& hand, float windowWidth, float windowHeight, Owner owner) {
    if (cards.empty())
        return false;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, cards.size() - 1);

    int idx = dist(gen);  // random indeks
    auto card = cards[idx];
    cards.erase(cards.begin() + idx);

    auto handCard = std::make_shared<CardHand>(
        card->getName(),
        *const_cast<sf::Texture*>(card->getTexture()),
        card->getHP(),
        card->getDamage(),
        card->getCost(),
        card->getRarity(),
        card->getBaseAttack(),
        card->getModifiers(),
        card->getHitCount(),
        card->getDescription()
    );

    handCard->setOwner(owner);
    hand.addCard(handCard, windowWidth, windowHeight);
    return true;
}