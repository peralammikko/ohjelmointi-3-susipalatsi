#include "withdrawagentaction.hh"
#include "agentitem.hh"
#include "playerhand.hh"

WithdrawAgentAction::WithdrawAgentAction(PlayerHand *hand, agentItem *aItem) : hand_(hand), aItem_(aItem)
{
 qDebug() << "WITHD11111RAW";
}

bool WithdrawAgentAction::canPerform() const
{
    qDebug() << "WITHDRAW";
    auto s =  hand_->getOwner();
     qDebug() << "1";
    auto l = aItem_->getObject()->owner().lock();
     qDebug() << "2";
     qDebug() << l->name();
    qDebug() << s->name();
    //qDebug() << s->name() << l->name();

    if (aItem_->parentItem() != hand_ and aItem_->getObject()->owner().lock() == hand_->getOwner())
    {
        return true;
    }
    return false;
}

void WithdrawAgentAction::perform()
{
    hand_->addMapItem(aItem_);
}
