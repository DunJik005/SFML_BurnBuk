#include "Hand.h"

// ---------------- INIT ----------------

void Hand::onResize(float windowWidth, float windowHeight) {
    float margin = windowHeight / 8.f;

    // baseY zavisi od toga da li je hand aktivan
    if (isActive)
        baseY = windowHeight - margin; // donji deo
    else
        baseY = margin;                // gornji deo

    // spacing zavisi od širine prozora
    spacing = windowWidth * 0.15f;

    recalcLayout(windowWidth); // pozicioniraj karte
}


// ---------------- LAYOUT ----------------

void Hand::recalcLayout(float windowWidth)
{
    if (cards.empty()) return;

    float totalWidth = (cards.size() - 1) * spacing;
    float startX = (windowWidth - totalWidth) / 2.f;

    for (size_t i = 0; i < cards.size(); i++)
    {
        cards[i]->handIndex = static_cast<int>(i);
        cards[i]->setSpritePosition(
            startX + i * spacing,
            baseY
        );
        std::cout << "Card " << i << " pozicija: " << startX + i*spacing << ", " << baseY << "\n";
    }
}

// ---------------- INPUT ----------------

std::shared_ptr<CardHand> Hand::handleClick(float x, float y)
{
    for (int i = (int)cards.size() - 1; i >= 0; i--)
    {
        if (!cards[i]) continue;
        if (cards[i]->getOwner() != visibleOwner) continue;

        if (cards[i]->containsPoint(x, y))
            return cards[i];
    }
    return nullptr;
}

// ---------------- MODIFY ----------------

void Hand::addCard(const std::shared_ptr<CardHand>& card, float windowWidth, float windowHeight)
{
    cards.push_back(card);
    float margin = windowHeight / 8.f;
    baseY = isActive ? windowHeight - margin : margin;
    recalcLayout(windowWidth);
}

void Hand::removeHand(const std::shared_ptr<CardHand>& card, float windowWidth)
{
    auto it = std::find(cards.begin(), cards.end(), card);
    if (it != cards.end())
        cards.erase(it);

    recalcLayout(windowWidth);
}


// ---------------- RENDER ----------------

void Hand::draw(sf::RenderWindow& window,
                const std::shared_ptr<CardHand>& selected)
{
    if (cards.empty()) return;

    // Za svaki draw, izračunaj baseY i spacing
    float margin = window.getSize().y / 8.f;   // windowHeight
    baseY = isActive ? window.getSize().y - margin : margin;

    spacing = window.getSize().x * 0.15f;      // windowWidth
    float totalWidth = (cards.size() - 1) * spacing;
    float startX = (window.getSize().x - totalWidth) / 2.f;

    for (size_t i = 0; i < cards.size(); i++)
    {
        auto& c = cards[i];
        if (!c) continue;

        c->handIndex = static_cast<int>(i);

        c->resetVisuals();

        if (!isActive) {
            // PROTIVNIKOV HAND → crtamo unazad
            c->getSprite().setTexture(CardHand::getCardBackTexture(), true);
            c->getSprite().setRotation(sf::degrees(180.f));
            c->setSpritePosition(startX + i * spacing, baseY); // **ovo je ključno**
            c->draw(window);
            continue;
        }

        // AKTIVAN HAND → crtamo front
        if (const sf::Texture* front = c->getTexture()) {
            c->getSprite().setTexture(*front, true);
        }
        c->getSprite().setRotation(sf::degrees(0.f));

        float y = baseY;

        if (c == selected)
        {
            y -= baseY / 10.f;
            c->setBrightness(true);
        }
        else if (selected)
        {
            c->setBrightness(false);
        }

        c->setSpritePosition(startX + i * spacing, y);  // **ovo je ključno**
        c->draw(window);
    }
}
