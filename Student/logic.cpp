#include "logic.hh"

Logic::Logic(std::shared_ptr<Interface::Runner> runner, std::shared_ptr<Interface::Game> game, GameScene* gameScene)
    : runner_(runner), game_(game), gameScene_(gameScene)
{
    connect(gameScene_.get(), &GameScene::actionDeclared, this, &Logic::actionSelected);
    connect(game_.get(), &Interface::Game::playerChanged, this, &Logic::onPlayerChanged);
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

void Logic::actionSelected(std::shared_ptr<Interface::ActionInterface> action)
{
    action_ = action;
    std::shared_ptr<Interface::ManualControl> manualCtrl = std::dynamic_pointer_cast<Interface::ManualControl>(ctrl_);
    if (manualCtrl)
    {
        manualCtrl->setNextAction(action_);
        doTheRunning();
        //game_->nextPlayer();
    } else {
        qDebug() << "Manual Control was not found";
        doTheRunning();
    }
}

void Logic::onPlayerChanged(std::shared_ptr<const Interface::Player> actingPlayer)
{
    // TODO: disable everything that should not move
    // Actually this is done in gamescene for now

    // BUG!!! This is called twice for some reason
    if (actingPlayer and game_->currentPlayer() != actingPlayer)
    {
        //qDebug() << game_->currentPlayer()->name() ;
        qDebug() << "ALERT LOGIC CHANGE";
        qDebug() << game_->currentPlayer()->name() << "changed to " << actingPlayer->name();

        // Get every card in hand that is type of Actioncard
        auto cards = actingPlayer->cards();
        std::vector<std::weak_ptr<const Interface::CardInterface>> actionCards;
        // Copies every card in player's cards vector to a new actioncards vector
        std::copy_if (cards.begin(), cards.end(), std::back_inserter(actionCards),
                      [](std::shared_ptr<const Interface::CardInterface> card){return card->typeName()=="actioncard";} );
        if (actionCards.size())
        {
            qDebug() << "Chaning player HAS action cards";
        } else {
            qDebug() << "Changing player DOES NOT have action cards";
        }
    }

}
