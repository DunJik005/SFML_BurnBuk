#include "GameController.h"


bool GameController::handleDeckClick(Hand& hand, float windowWidth, float windowHeight) {
    switch (currentPhase) {
        case GamePhase::Player1Place:
            std::cout << "[GC] Player1 vuče kartu\n";
            return deck.drawCardToHand(hand, windowWidth, windowHeight, getCurrentPlayer());

        case GamePhase::Player2Place:
            std::cout << "[GC] Player2 vuče kartu\n";
            return deck.drawCardToHand(hand, windowWidth, windowHeight, getCurrentPlayer());

        default:
            std::cout << "[GC] Ne može se vući karta u ovoj fazi\n";
            return false;
    }
}

void GameController::sendHandCardToGraveyard(Hand &hand, Graveyard &graveyard, std::shared_ptr<CardHand> &card, float windowWidth) {
    if (!card) return;
    graveyard.addCard(card);
    hand.removeHand(card, windowWidth);

    if (getCurrentPlayer() == Owner::Player1) {
        player1.addElixir();
        std::cout << "Player1 dobija elixir, trenutno: " << player1.getElixir() << "\n";
    } else if (getCurrentPlayer() == Owner::Player2) {
        player2.addElixir();
        std::cout << "Player2 dobija elixir, trenutno: " << player2.getElixir() << "\n";
    }

    card.reset();
}

void GameController::sendBoardCardToGraveyard(Tile& tile, Graveyard &graveyard, std::shared_ptr<Card> &card) {
    if (!card) return;
    if (tile.removeTopCard())
        graveyard.addCard(card);
    card.reset();

}



Owner GameController::getCurrentPlayer() const {
    switch (currentPhase)
    {
        case GamePhase::Player1Place: return Owner::Player1;
        case GamePhase::Player2Place: return Owner::Player2;
        default: return Owner::Player1; // ili None
    }
}


void GameController::updateHandsState(Hand& p1Hand, Hand& p2Hand)
{
    Owner current = getCurrentPlayer();

    if (current == Owner::Player1)
    {
        p1Hand.setActive(true);
        p2Hand.setActive(false);

        p1Hand.setVisibleOwner(Owner::Player1);
        p2Hand.setVisibleOwner(Owner::Player1);
    }
    else
    {
        p2Hand.setActive(true);
        p1Hand.setActive(false);

        p2Hand.setVisibleOwner(Owner::Player2);
        p1Hand.setVisibleOwner(Owner::Player2);
    }
}

void GameController::returnTopCardToHand(
    Tile& tile,
    Hand& hand,
    float windowWidth,
    float windowHeight
)
{


    Player& currentPlayer =
        (getCurrentPlayer() == Owner::Player1)
        ? player1
        : player2;
    // 1. Pravila

    if (!tile.canReturnCard(getCurrentPlayer()))
        return;

    // 2. Skini kartu sa tile-a
    auto card = tile.returnTopCardToHand();
    if (!card)
        return;

    currentPlayer.addElixir(card->getCost());
    std::cout << "Card elixir refund: " << card->getCost() << std::endl;

    // 3. ISTA LOGIKA KAO U DECK-u
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
    handCard->setOwner(card->getOwner());


    // 4. Dodaj u hand (sa width/height)
    hand.addCard(handCard, windowWidth, windowHeight);
}
