#include "InteractionController.h"
#include <iostream>

void InteractionController::handleLeftClick(
    int mouseX, int mouseY,
    Board& board,
    Deck& deck,
    Hand& activeHand,
    GameController& gameController,
    Graveyard& graveyard,
    Player& player1,
    Player& player2,
    float windowWidth,
    float windowHeight
) {

    // === RETURN BUTTON CLICK (HIGHEST PRIORITY) ===
    auto [r, c] = board.getTileAtPosition(mouseX, mouseY);
    if (r != -1)
    {
        Tile& tile = board.getTile(r, c);

        if (tile.canReturnCard(gameController.getCurrentPlayer()) &&
            tile.handleReturnClick(mouseX, mouseY))
        {
            gameController.returnTopCardToHand(
                tile,
                activeHand,
                windowWidth,
                windowHeight
            );
            clearSelection();
            return;
        }
    }


    // === KLIK NA DECK ===
    if (deck.contains((float)mouseX, (float)mouseY))
    {
        gameController.handleDeckClick(
            activeHand,
            windowWidth,
            windowHeight
        );
        clearSelection();
        return;
    }

    if (graveyard.contains((float)mouseX, (float)mouseY))
    {
        if (selectedHandCard)
        {
            gameController.sendHandCardToGraveyard(
                activeHand,
                graveyard,
                selectedHandCard,
                windowWidth
            );
            clearSelection();
            return;
        }

        if (selectedBoardCard)
        {
            gameController.sendBoardCardToGraveyard(
                board.getTile(selectedRow, selectedCol),
                graveyard,
                selectedBoardCard
            );
            clearSelection();
            return;
        }
    }

    // === KLIK NA HAND ===
    if (auto maybe = activeHand.handleClick((float)mouseX, (float)mouseY))
    {
        selectedHandCard = maybe;
        selectedBoardCard = nullptr;
        selectedRow = selectedCol = -1;
        return;
    }

    // === KLIK NA TILE ===
    auto [row, col] = board.getTileAtPosition(mouseX, mouseY);
    if (row != -1)
    {
        // Klik na postojeću kartu na boardu
        if (auto target = board.getTile(row, col).getAttackTarget())
        {
            selectedBoardCard = target;
            selectedRow = row;
            selectedCol = col;
            selectedHandCard = nullptr;
            return;
        }

        // === POSTAVLJANJE KARTE IZ HANDA ===
        if (selectedHandCard)
        {
            Player& currentPlayer =
                (gameController.getCurrentPlayer() == Owner::Player1)
                ? player1 : player2;

            int cost = selectedHandCard->getCost();
            if (!currentPlayer.spendElixir(cost))
            {
                std::cout << "Nemas dovoljno eliksira\n";
                return;
            }

            bool ok = board.placeCard(row, col, selectedHandCard);
            if (ok)
            {
                activeHand.removeHand(selectedHandCard, windowWidth);
                clearSelection(); // ✅ selekcija se briše TEK NAKON uspešne akcije
            }
            else
            {
                currentPlayer.addElixir(cost);
            }
            return;
        }
    }

    // === KLIK NA PRAZNO → reset selekcije ===
    clearSelection();
}


void InteractionController::handleRightClick(CardView& cardView)
{
    if (selectedHandCard)
        cardView.show(*selectedHandCard);
    else if (selectedBoardCard)
        cardView.show(*selectedBoardCard);
    else
        cardView.hide();
}

void InteractionController::clearSelection()
{
    selectedHandCard = nullptr;
    selectedBoardCard = nullptr;
    selectedRow = selectedCol = -1;
}

std::shared_ptr<CardHand> InteractionController::getSelectedHandCard() const {
    return selectedHandCard;
}

std::shared_ptr<Card> InteractionController::getSelectedBoardCard() const {
    return selectedBoardCard;
}
