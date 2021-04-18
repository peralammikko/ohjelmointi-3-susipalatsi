#include "logic.hh"

Logic::Logic(std::shared_ptr<Interface::Runner> runner, std::shared_ptr<Interface::Game> game) : runner_(runner), game_(game)
{
    connect(game_.get(), &Interface::Game::playerChanged, this, &Logic::playerChanged);
}

Logic::~Logic()
{

}

void Logic::doTheRunning()
{
    ctrl_ = runner_->run();
}

void Logic::actionSelected(std::shared_ptr<Interface::ActionInterface> action)
{
    qDebug() << "Hello I am logic and this is next action";
    action_ = action;
    if (ctrl_)
    {
        std::shared_ptr<Interface::ManualControl> manualCtrl = std::dynamic_pointer_cast<Interface::ManualControl>(ctrl_);
        manualCtrl->setNextAction(action_);
        doTheRunning();
    } else {
        doTheRunning();
    }

    // setNextAction();
}

void Logic::playerChanged(std::shared_ptr<const Interface::Player> actingPlayer) const
{
    // TODO: disable everything that should not move

}
