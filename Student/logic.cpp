#include "logic.hh"
#include <algorithm>
#include "ioexception.h"
#include "councilor.h"

Logic::Logic(std::shared_ptr<Interface::Runner> runner, std::shared_ptr<Interface::Game> game, GameScene* gameScene)
    : runner_(runner), game_(game), gameScene_(gameScene)
{
    connect(gameScene_.get(), &GameScene::actionDeclared, this, &Logic::onActionDeclared);
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

void Logic::rewardResources()
{
    std::vector<LocationItem*> items = gameScene_->GetLocItems();
    int resourceTotalAmount;
    agentItem* aItem;
    int rewardAmount = 1;
    for (auto player : game_->players()) {
        
        // Container for each unique location player has agents in (used to reward influence points)
        std::set<std::shared_ptr<Interface::Location>> locationsBeen = {};

        // Always give players one action card so they don't get stuck!
        std::shared_ptr<Interface::ActionCard> card = std::make_shared<Interface::ActionCard>();
        player->addCard(card);
        gameScene_->addActionCardForPlayer(player, card);

        for (auto card : player->cards()) {
            std::shared_ptr<Interface::Agent> agentPtr = std::dynamic_pointer_cast<Interface::Agent>(card);

            // If card is an agent card
            if (agentPtr) {
                std::shared_ptr<Interface::Location> agentAt = agentPtr->placement().lock();
                if (agentAt) {
                    // If agent is in any location (nullptr means no location a.k.a. "home"
                    if (agentAt != nullptr) {
                        resourceTotalAmount = 0;
                        locationsBeen.insert(agentAt);
                        Interface::CommonResource res = resMap_.at(agentAt);

                        // Find the correct location item for calculating rewards
                        for (auto loc : items)
                        {
                            if (loc->getObject()->id() == agentAt->id()) {
                                auto rewards = loc->calculateRewards(player);
                                rewardAmount = rewards.at(0);
                                aItem = loc->getAgentItemFor(agentPtr);
                                if (aItem == nullptr){
                                    throw Interface::IoException(QString("Agent "+agentPtr->name() +" could not have its agent item located during a parliamentary day"));
                                }
                            } else {
                               // throw Interface::IoException(QString("PD: Could not find location for "+agentPtr->name()));
                            }
                        }
                        for (int i = 0; i < rewardAmount; ++i)
                        {
                            if (agentAt->deck()->canDraw()){
                                auto drawnCard = agentAt->deck()->draw();
                                auto resu = std::dynamic_pointer_cast<Interface::CommonResource>(drawnCard);
                                if (resu)
                                {
                                    agentPtr->addResource(agentAt, res, resu->amount());
                                    resourceTotalAmount += resu->amount();
                                    agentAt->discards()->addCard(drawnCard);
                                } else {
                                    auto action = std::dynamic_pointer_cast<Interface::ActionCard>(drawnCard);
                                    if (action) {
                                        auto owner = agentPtr->owner().lock();
                                        owner->addCard(action);
                                        gameScene_->addActionCardForPlayer(owner, action);
                                    } else {
                                        throw Interface::IoException(QString("Agent "+agentPtr->name() +" drew an unsupported type card "+ drawnCard->typeName() +" in " + agentAt->name()));
                                    }
                                }
                            }
                        }
                        aItem->displayResourceChange(resourceTotalAmount, "");
                        // Agents earning resources passively by staying at location. Keep or not?

                        // agentPtr->addResource(agentAt, res, rewardAmount);


                        ///////////// CHEAT CODE ///////////////
                        //  agentPtr->addResource(agentAt, res, 100);
                        ////////////////////////////////////////

                    } else {
                        //throw Interface::IoException(QString("Agent "+agentPtr->name() +" could not be found during a parliamentary day"));
                    }
                } else {
                    qDebug() << agentPtr->name() << " not in any location";
                }
            }
        }
        for (auto loc : game_->locations()) {

            /////// CHEAT CODE ///////////
            // loc->setInfluence(player, 50);
            /////////////////////////////

            if (locationsBeen.find(loc) != locationsBeen.end()) {
                int inf = loc->influence(player);
                if (inf < 5) {
                    loc->setInfluence(player, inf+1);
                }
            }
        }
    }
}

void Logic::infoResourceMaps(ResourceMap &rmap, ResourceMap &dmap, int WINCOND)
{
    resMap_ = rmap;
    demandsMap_ = dmap;
    winCondition_ = WINCOND;
}

void Logic::checkWin()
{
    // Checking which player(s) holds at least 3 councilor cards
    std::set<std::shared_ptr<Interface::Player>> winners = {};
    for (auto player : game_->players()) {
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

        // Multiple winners allowed?
        for (auto player : winners) {
            qDebug() << "Long live " << player->name();
        }
    }
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
            emit(enteredEventPhase());
            rewardResources();
            gameScene_->nextRound();
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
                gameScene_->onPlayerChanged(actingPlayer);
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
    game_->nextPlayer();
}
