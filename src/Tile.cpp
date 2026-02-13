#include "Tile.h"
constexpr float STATS_RATIO = 0.15f;  // 15%
constexpr float ART_RATIO   = 0.85f;  // 85%

// ---------- Ctor ----------
Tile::Tile() : statsText(getFont()){
    shape.setFillColor(sf::Color::White);
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color::Black);
    statsBar.setFillColor(sf::Color::Black);
    statsText.setFillColor(sf::Color::White);

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
    updateStatsValue();
}

void Tile::updateStatsValue()
{
    if (cards.empty()) {
        statsText.setString("");
        return;
    }

    auto& card = cards.back(); // gornja živa karta

    statsText.setString(
        "DMG: " + std::to_string(card->getDamage()) +
        "  HP: "  + std::to_string(card->getHP())
    );
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

    if (type == PlacementType::Jelepeno)
        return true;


    switch (type)
    {
        case PlacementType::Normal:
        {
            if (!cards.empty()) {
                std::cout << "[canPlace] Normal: tile not empty\n";
                return false;
            }

            if (owner != cardOwner) {
                std::cout << "[canPlace] Normal: wrong side\n";
                return false;
            }

            return true;
        }

        case PlacementType::Infiltrator:
        {
            if (!cards.empty()) {
                std::cout << "[canPlace] Infiltrator: tile not empty\n";
                return false;
            }

            /*if (owner == Owner::None) {
                std::cout << "[canPlace] Infiltrator: neutral tile\n";
                return false;
            }*/

            if (owner == cardOwner) {
                std::cout << "[canPlace] Infiltrator: cannot place on own tile\n";
                return false;
            }

            return true;
        }

        case PlacementType::Parasite:
        {
            if (cards.empty()) {
                std::cout << "[canPlace] Parasite: no host card\n";
                return false;
            }

            /*if (owner == Owner::None) {
                std::cout << "[canPlace] Parasite: neutral tile\n";
                return false;
            }*/

            if (cards.front()->getOwner() == cardOwner) {
                std::cout << "[canPlace] Parasite: cannot attach to own card\n";
                return false;
            }

            return true;
        }
    }
    std::cout << "[canPlace] FAIL: unknown placement type\n";
    return false;
}

bool Tile::placeCard(const std::shared_ptr<Card>& card) {
    if (!canPlace(card))
        return false;

    cards.push_back(card);
    fitCardToTile(card);

    placedThisTurnCount++;

    return true;
}

bool Tile::canReturnCard(Owner currentPlayer) const {

    if (placedThisTurnCount <= 0)
        return false;

    if (cards.empty())
        return false;

    // gornja karta mora biti igraceva
    return cards.back()->getOwner() == currentPlayer;

}

std::shared_ptr<Card> Tile::removeTopCard() {
    if (cards.empty()) return nullptr;

    auto card = cards.back();
    cards.pop_back();
    return card;
}

std::shared_ptr<Card> Tile::returnTopCardToHand() {

    if (cards.empty() || placedThisTurnCount <= 0)
        return nullptr;

    auto card = cards.back();
    cards.pop_back();

    placedThisTurnCount--; // 👈 KLJUČNO
    return card;

}

std::shared_ptr<Card> Tile::getFirstEnemyCard(Owner owner) const
{
    // gornja → donja
    for (auto it = cards.rbegin(); it != cards.rend(); ++it)
    {
        if ((*it)->getOwner() != owner)
            return *it;
    }
    return nullptr;
}

const std::list<std::shared_ptr<Card>>& Tile::getAllCards() const
{
    return cards;
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

    if (hovered)
    {
        sf::RectangleShape overlay;
        overlay.setPosition(shape.getPosition());
        overlay.setSize(shape.getSize());
        overlay.setFillColor(sf::Color(255, 255, 255, 40)); // lagani glow

        window.draw(overlay);
    }

    // 3️⃣ crtaj karte potpuno normalno
    for (auto& card : cards) {
        window.draw(card->getSprite());
        window.draw(statsBar);
        window.draw(statsText);
    }
    if (!cards.empty() && canReturnCard(cards.back()->getOwner()))
    {
        returnButton.setBounds(cards.back()->getBounds());
        returnButton.setVisible(true);
        returnButton.draw(window);
    }
    else
    {
        returnButton.setVisible(false);
    }
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

    sf::Vector2f tilePos  = shape.getPosition();
    sf::Vector2f tileSize = shape.getSize();

    float statsHeight = tileSize.y * STATS_RATIO;

    statsBar.setPosition(tilePos);
    statsBar.setSize({ tileSize.x, statsHeight });
    statsText.setString(
    "DMG: " + std::to_string(card->getDamage()) +
    "  HP: "  + std::to_string(card->getHP())
);
    unsigned charSize = static_cast<unsigned>(statsHeight * 0.9f);
    statsText.setCharacterSize(charSize);
    sf::FloatRect textBounds = statsText.getLocalBounds();

    statsText.setPosition({
        tilePos.x + (tileSize.x - textBounds.size.x) / 2.f - textBounds.position.x,
        tilePos.y + (statsHeight - textBounds.size.y) / 2.f - textBounds.position.y
    });


    card->applyVisualMode();
    sf::Sprite& sprite = card->getSprite();
    const sf::FloatRect bounds = sprite.getLocalBounds();

    float artHeight = tileSize.y * ART_RATIO;
    float artTop    = tilePos.y + statsHeight;
    // scale da stane u 80%
    float scaleX = tileSize.x / bounds.size.x;
    float scaleY = artHeight   / bounds.size.y;
    float scale  = std::min(scaleX, scaleY);

    sprite.setScale({ scale, scale });

    // centriranje u donjem delu
    sprite.setPosition({
        tilePos.x + (tileSize.x - bounds.size.x * scale) / 2.f,
        artTop    + (artHeight - bounds.size.y * scale) / 2.f
    });
}

sf::Vector2f Tile::getPosition() const {
    return shape.getPosition();
}

sf::Vector2f Tile::getSize() const {
    return shape.getSize();
}

bool Tile::handleReturnClick(float x, float y) const
{
    return returnButton.contains(x, y);
}


sf::Font& Tile::getFont() {
    static sf::Font font;
    static bool initialized = false;

    if (!initialized) {
        if (!font.openFromFile("assets/fonts/MatrixSmallCaps.ttf")) {
            std::cerr << "greska: font nije ucitan\n";
        }
        initialized = true;
    }
    return font;
}