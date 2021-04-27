#include "logic.hh"
#include <algorithm>
#include "ioexception.h"
#include "councilor.h"

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
        qDebug() << "Error: Logic tried to launch the game while it was already running";
        return;
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
        // LOCK GAME SOMEHOW
    }
    return winners;
}

void Logic::onActionDeclared(std::shared_ptr<Interface::ActionInterface> action)
{
    action_ = action;
    std::shared_ptr<Interface::ManualControl> manualCtrl = std::dynamic_pointer_cast<Interface::ManualControl>(ctrl_);
    if (manualCtrl)
    {
        qDebug() << "Manual Control was found";
        manualCtrl->setNextAction(action_);
        doTheRunning();
    } else {
        qDebug() << "Manual Control was not found";
        // AI?
        doTheRunning();
    }
}

void Logic::onPlayerChanged(std::shared_ptr<const Interface::Player> actingPlayer)
{    
    if (actingPlayer and game_->currentPlayer() != actingPlayer)
    {
        // If we are just looking for a player who has action cards but could not find one, proceed to even phase
        if (actingPlayer_ != nullptr and actingPlayer_ == actingPlayer){
            // "We have gone a full circle of players with no action cards. Proceeding to event phase"
            checkWin(game_->players());

            emit(enteredEventPhase());
            emit(readyToRewardResources());
            emit(enteringNextRound());
            actingPlayer_ = nullptr;
            game_->nextPlayer();
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
                qDebug() << "Chaning player DOES NOT HAVE action cards";
                if (not actingPlayer_){
                    actingPlayer_ = actingPlayer;
                }
                game_->nextPlayer();
            }
        }
    }
    
}

void Logic::deckChanged(std::shared_ptr<const Interface::Location> location) const
{
    if (!location->deck()->canDraw()){
        if(!location->discards()->canDraw()){
            // Here I would shuffle the discards back to the main deck, but the signal gives me a const which I can't change unless I am to break a contract, and location doesn't have a reshuffle method anyway.
            // So I guess I wont Reshuffle the deck and it will eventually dry out. Too bad! I won't even connect this slot
            //while(location->discards()->size()){
            //    auto card = location->discards()->cardAt(0);

           // }
        }
    }
}

void Logic::onActionPerformed(std::shared_ptr<const Interface::Player> player, std::shared_ptr<Interface::ActionInterface> action)
{
    // Signal the gamewindow to disable gameview for a while.
    emit(requestInterphase(1200));
    game_->nextPlayer();
    game_->setActive(false);
    //gameScene_->onPlayerChanged(game);
    actingPlayer_ = nullptr;

}

void Logic::onInterphaseTimeout()
{
    game_->setActive(true);

    doTheRunning();
}
