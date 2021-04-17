#include "logic.hh"

Logic::Logic(std::shared_ptr<Interface::Runner> runner) : runner_(runner)
{

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
        qDebug() << "Sending to making manualtctrl";
        std::shared_ptr<Interface::ManualControl> manualCtrl = std::dynamic_pointer_cast<Interface::ManualControl>(ctrl_);
        qDebug() << "next action";
        manualCtrl->setNextAction(action_);
        doTheRunning();
    } else {
        qDebug() << "there was no ctrl. Running";
        doTheRunning();
    }

   // setNextAction();
}

void Logic::setNextAction()
{

}
