#include "withdrawagentaction.hh"
#include "agentitem.hh"
#include "playerhand.hh"

WithdrawAgentAction::WithdrawAgentAction(PlayerHand *hand, agentItem *aItem) : hand_(hand), aItem_(aItem)
{

}

bool WithdrawAgentAction::canPerform() const
{
    if (aItem_->parentItem() != hand_ and aItem_->getAgentClass()->owner().lock() == hand_->getOwner())
    {
        return true;
    }
    return false;
}

void WithdrawAgentAction::perform()
{
    QPointF currentPos = aItem_->scenePos();
    aItem_->setPos(hand_->mapFromScene(currentPos));
    hand_->addMapItem(aItem_);
}
