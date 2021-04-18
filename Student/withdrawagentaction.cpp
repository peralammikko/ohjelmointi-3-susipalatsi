#include "withdrawagentaction.hh"
#include "agentitem.hh"
#include "playerhand.hh"

WithdrawAgentAction::WithdrawAgentAction(PlayerHand *hand, agentItem *aItem) : hand_(hand), aItem_(aItem)
{

}

bool WithdrawAgentAction::canPerform() const
{
    if (aItem_->parentItem() != hand_)
    {
        return true;
    }
    return false;
}

void WithdrawAgentAction::perform()
{
    hand_->addMapItem(aItem_);
}
