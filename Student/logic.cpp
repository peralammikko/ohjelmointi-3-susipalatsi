#include "logic.hh"
#include <algorithm>

Logic::Logic(std::shared_ptr<Interface::Runner> runner, std::shared_ptr<Interface::Game> game, GameScene* gameScene)
    : runner_(runner), game_(game), gameScene_(gameScene)
{
    connect(gameScene_.get(), &GameScene::actionDeclared, this, &Logic::actionSelected);
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

    // qDebug() << "Reward Resources  in gamewindow - TODO: maybe move to logic";
    int rewardAmount = 1;
    for (auto player : game_->players()) {

        for (auto card : player->cards()) {
            std::shared_ptr<Interface::Agent> agentPtr = std::dynamic_pointer_cast<Interface::Agent>(card);

            // If card is an agent card
            if (agentPtr) {
                std::shared_ptr<Interface::Location> agentAt = agentPtr->placement().lock();

                if (agentAt) {
                    // If agent is in any location (nullptr means no location a.k.a. "home"
                    if (agentAt != nullptr) {

                        // Find the correct location item for calculating rewards
                        for (auto loc : items) {
                            if (loc->getObject()->id() == agentAt->id()) {
                                auto rewards = loc->calculateRewards(player);
                                rewardAmount = rewards.at(0);
                            }
                        }
                        Interface::CommonResource res = resMap_.at(agentAt);

                        // Set it for 1 resource per reward until calculateRewards() is moved
                        agentPtr->addResource(agentAt, res, rewardAmount);
                    } else {
                        qDebug() << "Agent not found";
                    }
                } else {
                    qDebug() << agentPtr->name() << " not in any location";
                }
            }
        }
    }
}

void Logic::infoResourceMaps(ResourceMap &rmap, ResourceMap &dmap)
{
    resMap_ = rmap;
    demandsMap_ = dmap;
}

void Logic::actionSelected(std::shared_ptr<Interface::ActionInterface> action)
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
            qDebug() << "We have gone a full circle of players with no action cards. Proceeding to event phase";
            rewardResources();
            gameScene_->nextRound();
            actingPlayer_ = nullptr;
            game_->nextPlayer();

        } else {
                std::vector<std::weak_ptr<const Interface::CardInterface>> actionCards;
                auto currentPlayerCards = game_->currentPlayer()->cards();
                qDebug() << game_->currentPlayer()->name() << "changed to " << actingPlayer->name();

                // Copies every card in player's cards vector to a new actioncards vector
                std::copy_if (currentPlayerCards.begin(), currentPlayerCards.end(), std::back_inserter(actionCards),
                              [](std::shared_ptr<const Interface::CardInterface> card){return card->typeName()=="actioncard";} );
                if (actionCards.size())
                {
                    qDebug() << "Chaning player HAS action cards";
                    gameScene_->onPlayerChanged(actingPlayer);
                    actingPlayer_ = nullptr;
                } else {
                    // If changing player has no action cards, start searching for a player who does.
                    // If one is not found (actingPlayer_ == actingPlayer) proceed to event phase
                    qDebug() << "Chaning player DOES NOT HAVE action cards";
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
    game_->nextPlayer();
    std::shared_ptr<Interface::Player> current = game_->currentPlayer();
}
