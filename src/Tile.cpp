#include "Tile.h"

// ---------- Ctor ----------
Tile::Tile() {
    shape.setFillColor(sf::Color::White);
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color::Black);
}

// ---------- State / Ownership ----------
void Tile::setState(State s) {
    state = s;

    if (state == State::Inactive)
        shape.setFillColor(sf::Color(120, 120, 120));
    else
        shape.setFillColor(sf::Color(200, 200, 220));
}

Tile::State Tile::getState() const {
    return state;
}

void Tile::setOwner(Owner o) {
    owner = o;
}

Owner Tile::getOwner() const {
    return owner;
}

bool Tile::isActive() const {
    return state == State::Active;
}

// ---------- Card stack ----------
bool Tile::empty() const {
    return cards.empty();
}

size_t Tile::cardCount() const {
    return cards.size();
}

std::shared_ptr<Card> Tile::topCard() const {
    if (cards.empty()) return nullptr;
    return cards.back();
}

std::shared_ptr<Card> Tile::bottomCard() const {
    if (cards.empty()) return nullptr;
    return cards.front();
}

bool Tile::hasLeech() const {
    return cards.size() > 1;
}

void Tile::cleanupDeadCards()
{
    if (cards.empty()) return;

    if (cards.front()->getHP() <= 0) {
        cards.clear();
        return;
    }

    for (auto it = cards.begin(); it != cards.end(); ) {
        if ((*it)->getHP() <= 0) {
            it = cards.erase(it);
        } else {
            ++it;
        }
    }
}




// ---------- Placement rules ----------
bool Tile::canPlace(const std::shared_ptr<Card>& card) const
{
    if (!card) {
        std::cout << "[canPlace] FAIL: null card\n";
        return false;
    }

    if (!isActive()) {
        std::cout << "[canPlace] FAIL: tile inactive\n";
        return false;
    }

    if (card->getIgnoresTileRules())
        return true;

    if (owner == Owner::None) {
        std::cout << "[canPlace] FAIL: neutral tile\n";
        return false;
    }

    Owner cardOwner = card->getOwner();
    PlacementType type = card->getPlacementType();

    switch (type)
    {
        case PlacementType::Normal:
            if (!cards.empty()) {
                std::cout << "[canPlace] FAIL: normal card but tile not empty\n";
                return false;
            }
            if (owner != cardOwner) {
                std::cout << "[canPlace] FAIL: normal card on enemy tile\n";
                return false;
            }
            return true;

        case PlacementType::Parasite:
            if (cards.empty()) {
                std::cout << "[canPlace] FAIL: parasite needs host card\n";
                return false;
            }
            if (owner == cardOwner) {
                std::cout << "[canPlace] FAIL: parasite on own tile\n";
                return false;
            }
            return true;

        case PlacementType::Infiltrator:
            if (!cards.empty()) {
                std::cout << "[canPlace] FAIL: infiltrator requires empty tile\n";
                return false;
            }
            if (owner == cardOwner) {
                std::cout << "[canPlace] FAIL: infiltrator on own tile\n";
                return false;
            }
            return true;
    }

    std::cout << "[canPlace] FAIL: unknown placement type\n";
    return false;
}

bool Tile::placeCard(const std::shared_ptr<Card>& card) {
    if (!canPlace(card))
        return false;

    cards.push_back(card);
    fitCardToTile(card);

    return true;
}

std::shared_ptr<Card> Tile::removeTopCard() {
    if (cards.empty()) return nullptr;

    auto card = cards.back();
    cards.pop_back();
    return card;
}

// ---------- Visual ----------

void Tile::setTexture(sf::Texture* texture)
{
    shape.setTexture(texture);
}

void Tile::setHovered(bool h)
{
    hovered = h;
}


void Tile::setPosition(float x, float y) {
    shape.setPosition({x, y});

    for (auto& card : cards)
        fitCardToTile(card);
}

void Tile::setSize(float w, float h) {
    shape.setSize({ w, h });

    for (auto& card : cards)
        fitCardToTile(card);
}

sf::FloatRect Tile::getBounds() const
{
    if (state == State::Inactive)
        return sf::FloatRect();

    return shape.getGlobalBounds();
}

void Tile::draw(sf::RenderWindow& window)
{
    if (state == State::Inactive)
        return;

    // 1️⃣ crtaj tile normalno
    window.draw(shape);

    // 2️⃣ hover glow (overlay, NE fillColor)
    if (hovered)
    {
        sf::RectangleShape overlay;
        overlay.setPosition(shape.getPosition());
        overlay.setSize(shape.getSize());
        overlay.setFillColor(sf::Color(255, 255, 255, 40)); // lagani glow

        window.draw(overlay);
    }

    // 3️⃣ crtaj karte potpuno normalno
    for (auto& card : cards)
        card->draw(window);
}

std::shared_ptr<Card> Tile::getAttackTarget() {
    if (cards.empty())
        return nullptr;

    // uvek napadamo gornju kartu
    return cards.back();
}

// ---------- Internal ----------
void Tile::fitCardToTile(const std::shared_ptr<Card>& card) {
    if (!card) return;

    sf::Sprite& sprite = card->getSprite();
    const sf::FloatRect bounds = sprite.getLocalBounds();

    sprite.setPosition(shape.getPosition());

    sprite.setScale({
        shape.getSize().x / bounds.size.x,
        shape.getSize().y / bounds.size.y
    });
}


sf::Vector2f Tile::getPosition() const {
    return shape.getPosition();
}

sf::Vector2f Tile::getSize() const {
    return shape.getSize();
}
