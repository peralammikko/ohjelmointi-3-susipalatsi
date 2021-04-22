#include "passaction.hh"


PassAction::PassAction(PlayerHand *hand) : hand_(hand)
{

}

bool PassAction::canPerform() const
{
    return true;
}

void PassAction::perform()
{
    hand_->removeActionCards();
    hand_->rearrange();
}

mapItem *PassAction::getTargetMapItem()
{
    return hand_;
}

QString PassAction::pastTenseDescription()
{
    return "passed and discarded their hand";
}
