#include "passaction.hh"


PassAction::PassAction(std::shared_ptr<Interface::Player> player) : player_(player)
{

}

bool PassAction::canPerform() const
{
    return true;
}

void PassAction::perform()
{

}

mapItem *PassAction::getTargetMapItem()
{
    return nullptr;
}

QString PassAction::pastTenseDescription()
{
    return "passed and discarded their hand";
}
