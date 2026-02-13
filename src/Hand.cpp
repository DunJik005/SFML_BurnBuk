#include "Hand.h"
// ---------------- INIT ----------------
void Hand::onResize(float windowWidth, float windowHeight)
{
    lastWindowWidth = windowWidth;
    lastWindowHeight = windowHeight;

    centerX = windowWidth * 0.5f;
    zoneHeight = windowHeight * 3.f / 10.f;

    if (cards.empty()) return;

    // skala se računa iz visine zone
    float cardTexH = cards[0]->getSprite().getLocalBounds().size.y;

    // želimo da se vidi ~50% karte
    float desiredVisible = zoneHeight * 0.9f;
    cardScale = desiredVisible / (cardTexH * 0.5f);

    for (auto& c : cards)
        c->getSprite().setScale({cardScale, cardScale});

    float cardW = cards[0]->getSprite().getGlobalBounds().size.x;
    float cardH = cards[0]->getSprite().getGlobalBounds().size.y;

    float spacingX = cardW * overlapRatio;
    float totalW = cardW + (cards.size() - 1) * spacingX;
    float startX = centerX - totalW * 0.5f;

    // Y pozicija zavisi od toga da li je hand aktivan
    if (isActive)
    {
        // donja – centar karte malo iznad dna zone
        baseY = windowHeight - zoneHeight * 0.5f;
    }
    else
    {
        // gornja
        baseY = zoneHeight * 0.5f - cardH * 0.5f;
    }

    for (size_t i = 0; i < cards.size(); ++i)
    {
        cards[i]->setHandIndex((int)i);
        cards[i]->getSprite().setPosition( { startX + i * spacingX, baseY });
        std::cout << "Card " << i << " pozicija: " << startX + i*spacingX << ", " << baseY << "\n";

    }
}

// ---------------- Handle Click ----------------

std::shared_ptr<CardHand> Hand::handleClick(float x, float y)
{
    if (!isActive) return nullptr;

    for (int i = (int)cards.size() - 1; i >= 0; i--)
    {
        if (!cards[i]) continue;
        if (cards[i]->getOwner() != visibleOwner) continue;

        if (cards[i]->containsPoint(x, y)) {
            std::cout << "Pozicija klika:" << x << ", " << y << ", Selectovana karta " << i <<"\n";
            return cards[i];
        }
    }
    return nullptr;
}

// ---------------- Modify ----------------

void Hand::addCard(const std::shared_ptr<CardHand>& card, float windowWidth, float windowHeight)
{

    card->getSprite().setScale({cardScale, cardScale});
    cards.push_back(card);
    onResize(windowWidth ,windowHeight);
}

void Hand::removeHand(const std::shared_ptr<CardHand>& card)
{
    for (auto it = cards.begin(); it != cards.end(); ++it)
    {
        if (*it == card)
        {
            cards.erase(it);
            break;
        }
    }
    onResize(lastWindowWidth, lastWindowHeight);
}

// ---------- Active Hand ----------
void Hand::setActive(bool active)
{
    if (isActive == active) return;

    isActive = active;

    // odmah prerasporedi karte
    onResize(lastWindowWidth, lastWindowHeight);
}

// ---------------- Draw ----------------
void Hand::draw(sf::RenderWindow& window, const std::shared_ptr<CardHand>& selected)
{
    for (auto& c : cards)
    {
        if (!c)
            continue;
        if (!isActive)
            c->getCardRenderer().setRenderMode(RenderMode::Back);
        else
            c->getCardRenderer().setRenderMode(RenderMode::Front);

        float y = baseY;

        if (c == selected)
        {
            y -= cards[0]->getHeight() * 0.1f; // karta “izađe” gore
            c->setBrightness(true);
        }
        else
        {
            c->setBrightness(false);
        }

        c->setSpritePosition(c->getSprite().getPosition().x, y);

        c->draw(window);
    }
}