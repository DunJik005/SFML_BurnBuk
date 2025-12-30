#include "Hand.h"

// ---------------- INIT ----------------

void Hand::onResize(float windowWidth, float windowHeight) {
    if (cards.empty()) return;


    /* OVDE CE DA SE MENJA BASEY PO ACTIVEU */
    baseY = windowHeight - windowWidth / 8.f;

    spacing = windowWidth * 0.15f;

    float totalWidth = (cards.size() - 1) * spacing;

    float startX = (windowWidth - totalWidth) / 2.f;

    for (size_t i = 0; i < cards.size(); i++) {
        cards[i]->handIndex = static_cast<int>(i);
        cards[i]->setSpritePosition(startX + i * spacing, baseY);
    }
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
    baseY = windowHeight - windowWidth / 8.f;
    recalcLayout(windowWidth);
}

void Hand::removeHand(const std::shared_ptr<CardHand>& card, float windowWidth)
{
    for (auto it = cards.begin(); it != cards.end(); ++it)
    {
        if (*it == card)
        {
            cards.erase(it);
            break;
        }
    }
    recalcLayout(windowWidth);
}

// ---------------- RENDER ----------------

void Hand::draw(sf::RenderWindow& window,
                const std::shared_ptr<CardHand>& selected)
{
    for (auto& c : cards)
    {
        if (!c)
            continue;

        c->resetVisuals();

/*        HAND DRUGI NAOPAK
        if (!isActive) {
            c->getSprite().setTexture(CardHand::getCardBackTexture(), true);
            c->getSprite().setRotation(sf::degrees(180.f));
            c->draw(window);
            continue;
        }
        else {
            // VRATI FRONT
            c->getSprite().setTexture(c->getFrontTexture(), true);
            c->getSprite().setRotation(sf::degrees(0.f));
        }
*/
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

        c->setSpritePosition(c->getSprite().getPosition().x, y);
        c->draw(window);
    }
}
