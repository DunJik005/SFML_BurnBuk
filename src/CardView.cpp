#include "CardView.h"

CardView::CardView() {}

void CardView::show(const Card& c) {
    card = &c;
    visible = true;
    renderer.setCard(c);

    if (lastWinW > 0 && lastWinH > 0)
        onResize(lastWinW, lastWinH);
}

void CardView::hide() {
    visible = false;
    card = nullptr;
}

void CardView::onResize(float winW, float winH) {
    lastWinW = winW;
    lastWinH = winH;
    if (!visible || !card) return;

    float panelW = winW * 0.33f - margin * 2.f;
    float panelH = winH - margin * 2.f;

    auto texSize = Card::getRarityFrame(card->getRarity()).getSize();
    float cardW = texSize.x;
    float cardH = texSize.y;

    scaleFactor = std::min(panelW / cardW, panelH / cardH);

    panelPos.x = winW * 0.67f + margin;
    panelPos.y = (winH - cardH * scaleFactor) * 0.5f;

    renderer.setScale(scaleFactor);
    renderer.setPosition(panelPos);
}
bool CardView::isVisible() const {
    return visible;
}

void CardView::scrollDescription(float delta) {
    if (!visible) return;
    renderer.scrollDescription(delta);
}

void CardView::update(float dt) {
    if (!visible) return;
    renderer.update(dt);
}

void CardView::draw(sf::RenderWindow& window) const {
    if (!visible || !card) return;
    renderer.draw(window);
}
