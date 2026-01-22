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

    if (!deckTexture.loadFromFile("assets/deckic.png")) {
        std::cerr << "Ne mogu da ucitam deckic.png\n";
    } else {
        deckSprite.setTexture(deckTexture);
    }

    std::cout << "Deck ima " << cards.size() << " karata.\n";
}

void Deck::setPosition(float x, float y) {
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