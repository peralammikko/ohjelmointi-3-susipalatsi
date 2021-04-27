#include "withdrawagentaction.hh"


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
    auto agent = aItem_->getAgentClass();
    auto locationItem = dynamic_cast<LocationItem*>(aItem_->parentItem());
    if (locationItem){
        locationItem->getObject().get()->removeAgent(agent);
    }
    QPointF currentPos = aItem_->scenePos();
    aItem_->setPos(hand_->mapFromScene(currentPos));
    hand_->addMapItem(aItem_);

    aItem_->getAgentClass()->owner().lock()->addCard(agent);

    if (agent->hasCouncilCard()) {
        agent->owner().lock()->addCard(agent->getCouncilCard());
        agent->addCouncilCard(nullptr);
    }
}

mapItem *WithdrawAgentAction::getTargetMapItem()
{
    return hand_;
}

QString WithdrawAgentAction::pastTenseDescription()
{
    return "withdrew " + aItem_->getAgentClass()->name();
}
