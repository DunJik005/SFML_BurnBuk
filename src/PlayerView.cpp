#include "PlayerView.h"

PlayerView::PlayerView(
    sf::RenderWindow& w,
    Owner myOwner,
    Board& b,
    Hand& my,
    Hand& enemy,
    Deck& d,
    Graveyard& g,
    TurnButtons& btn,
    GameController& gc,
    InteractionController& ic,
    CardView& cv,
    Player& p1,
    Player& p2
)
    : window(w),
      owner(myOwner),
      board(b),
      myHand(my),
      enemyHand(enemy),
      deck(d),
      graveyard(g),
      buttons(btn),
      controller(gc),
      interaction(ic),
      cardView(cv),
      player1(p1),
      player2(p2)
{
    view = window.getDefaultView();
}


sf::Vector2f PlayerView::mapMouse(int x, int y) {
    return window.mapPixelToCoords({x, y});
}

void PlayerView::handleEvent(const sf::Event& e) {

    if (auto* m = e.getIf<sf::Event::MouseButtonPressed>()) {

        auto world = mapMouse(m->position.x, m->position.y);

        if (controller.getCurrentPlayer() == owner) {

            if (buttons.handleClick((int)world.x, (int)world.y, owner == Owner::Player1)) {
                if (owner == Owner::Player1)
                    controller.setPlayer1Done(true);
                else
                    controller.setPlayer2Done(true);

                controller.update();
                return;
            }

            if (m->button == sf::Mouse::Button::Left) {
                interaction.handleLeftClick(
                (int)world.x, (int)world.y,
                board, deck, myHand,
                controller, graveyard,
                player1, player2,
                window.getSize().x,
                window.getSize().y
            );

            }
        }

        if (m->button == sf::Mouse::Button::Right)
            interaction.handleRightClick(cardView);
    }

    if (auto* sc = e.getIf<sf::Event::MouseWheelScrolled>())
        cardView.scrollDescription(sc->delta);

    if (auto* r = e.getIf<sf::Event::Resized>()) {
        board.onResize(r->size.x, r->size.y);
        buttons.onResize(r->size.x, r->size.y);
        deck.setPosition(board, -80.f);
        graveyard.setPosition(board, 80);
        myHand.onResize(r->size.x, r->size.y);
        enemyHand.onResize(r->size.x, r->size.y);
        cardView.onResize(r->size.x, r->size.y);

        window.setView(sf::View(sf::FloatRect(
            {0.f, 0.f},
            {(float)r->size.x, (float)r->size.y}
        )));
    }
}

void PlayerView::update(float dt) {
    cardView.update(dt);
}

void PlayerView::draw() {
    bool myTurn = (controller.getCurrentPlayer() == owner);

    // moj hand: dole kad je moj POV, gore kad je protivnički
    myHand.setActive(true);
    myHand.setVisibleOwner(owner);

    // protivnički hand: uvek skriven i gore
    enemyHand.setActive(false);
    enemyHand.setVisibleOwner(owner);

    // rasporedi ih
    myHand.onResize(window.getSize().x, window.getSize().y);
    enemyHand.onResize(window.getSize().x, window.getSize().y);

    board.draw(window, interaction.getSelectedBoardCard(), controller.getCurrentPlayer());
    enemyHand.draw(window, nullptr);
    myHand.draw(window, interaction.getSelectedHandCard());
    deck.draw(window);
    graveyard.draw(window);
    cardView.draw(window);
    buttons.draw(window);
}
