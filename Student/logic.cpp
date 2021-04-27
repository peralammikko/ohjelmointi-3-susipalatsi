#include "logic.hh"
#include <algorithm>
#include "stateexception.h"
#include "councilor.h"
#include "../Course/deck.h"
#include "deck.h"

Logic::Logic(std::shared_ptr<Interface::Runner> runner, std::shared_ptr<Interface::Game> game)
    : runner_(runner), game_(game)
{

}

Logic::~Logic()
{

}

void Logic::doTheRunning()
{
    ctrl_ = runner_->run();
}

void Logic::launchGame()
{
    if (!game_->active())
    {

    } else {

        throw Interface::StateException(QString("Error: Logic tried to launch the game while it was already running"));
    }
    game_->setActive(true);
}

std::set<std::shared_ptr<Interface::Player>> Logic::checkWin(std::vector<std::shared_ptr<Interface::Player> > players)
{
    // Checking which player(s) holds councilor cards according to win condition
    std::set<std::shared_ptr<Interface::Player>> winners = {};
    for (auto player : players) {
        int cardCount = 0;
        for (auto card : player->cards()) {
            std::shared_ptr<Interface::Councilor> councilCard = std::dynamic_pointer_cast<Interface::Councilor>(card);
            if (councilCard) {
                cardCount++;
            }
        }
        if (cardCount >= winCondition_) {
            winners.insert(player);
        }
    }
    if (winners.size() > 0) {
        emit onWinnersFound(winners);
    }
    return winners;
}

void Logic::onActionDeclared(std::shared_ptr<Interface::ActionInterface> action)
{
    action_ = action;
    std::shared_ptr<Interface::ManualControl> manualCtrl = std::dynamic_pointer_cast<Interface::ManualControl>(ctrl_);
    if (manualCtrl)
    {
        manualCtrl->setNextAction(action_);
        doTheRunning();
    } else {
        doTheRunning();
    }
}

void Logic::onPlayerChanged(std::shared_ptr<const Interface::Player> actingPlayer)
{    
    if (actingPlayer and game_->currentPlayer() != actingPlayer)
    {
        // If we are just looking for a player who has action cards but could not find one, proceed to even phase
        if (actingPlayer_ != nullptr and actingPlayer_ == actingPlayer){
            // We have gone a full circle of players with no action cards. Proceeding to event phase
            auto winners = checkWin(game_->players());
            if (!winners.size()){
                emit(readyToRewardResources());
                emit(enteredEventPhase());
                reshuffleLocationDecks();
                actingPlayer_ = nullptr;
                game_->nextPlayer();
            } else {
                return;
            }
        } else {
            // check if new player has action cards
            std::vector<std::weak_ptr<const Interface::CardInterface>> actionCards;
            auto currentPlayerCards = game_->currentPlayer()->cards();
            std::copy_if (currentPlayerCards.begin(), currentPlayerCards.end(), std::back_inserter(actionCards),
                          [](std::shared_ptr<const Interface::CardInterface> card){return card->typeName()=="actioncard";} );
            if (actionCards.size())
            {
                //gameScene_->onPlayerChanged(actingPlayer);
                actingPlayer_ = nullptr;
            } else {
                // "Chaning player DOES NOT HAVE action cards";
                if (not actingPlayer_){
                    actingPlayer_ = actingPlayer;
                }
                game_->nextPlayer();
            }
        }
    }
    
}

void Logic::onActionPerformed(std::shared_ptr<const Interface::Player> player, std::shared_ptr<Interface::ActionInterface> action)
{
    emit(requestInterphase(1200));
    game_->nextPlayer();
    game_->setActive(false);
    actingPlayer_ = nullptr;
}

void Logic::onInterphaseTimeout()
{
    game_->setActive(true);
    doTheRunning();
}

void Logic::reshuffleLocationDecks()
{
    auto locs = game_->locations();
    for (auto loc : locs){
        if (!loc->deck()->canDraw()){
            auto discards = loc->discards();
            while (discards->canDraw()){
                auto card = discards->draw();
                loc->deck()->addCard(card);
            }
            loc->deck()->shuffle();
        }
    }
}
