#include "sendagentaction.hh"
#include "agentitem.hh"
#include "locationitem.hh"


SendAgentAction::SendAgentAction(LocationItem* newLocItem, agentItem* aItem) : newLocItem_(newLocItem), aItem_(aItem)
{

}

SendAgentAction::~SendAgentAction(){}

bool SendAgentAction::canPerform() const
{
    auto oldLocItem = dynamic_cast<LocationItem*>(aItem_->parentItem());
    if (!oldLocItem)
    {
        // TODO: require payment
        // qDebug() << "attempting move an agent from hand to place";
       return true;
    } else if (oldLocItem != newLocItem_){
       // oldPlacInterface->removeAgent(aInterface);
        // qDebug() << "attempting move an agent from place to place";
        // Calculate the distances between locations
        int dist = abs(newLocItem_->mapIndex() - oldLocItem->mapIndex());
        // TODO: maybe implement movements which are larger than one
        // BUG TODO: from max index to min index jumping
        if ( dist == 1 )
        {
          // TODO: maybe additional pooling
          return true;
        }
    }
    // Action can not be performed
    aItem_->goHome();
    return false;
}

void SendAgentAction::perform()
{
    std::shared_ptr<Interface::Agent> aInterface = aItem_->getAgentClass();
    std::shared_ptr<Interface::Location> newPlacInterface = newLocItem_->getObject();
    std::shared_ptr<Interface::Location> oldPlacInterface = aInterface->placement().lock();

    // Removes agent from its previous location, sends the agent to new location and sets new "home coords"
    if (oldPlacInterface){
        oldPlacInterface->removeAgent(aInterface);
    }
    newPlacInterface->sendAgent(aInterface);

    // Convert current position on scene to new parent item's coordinates
    // Do this so that the item does not fly far far away
    QPointF currentPos = aItem_->scenePos();
    aItem_->setPos(newLocItem_->mapFromScene(currentPos));

    aItem_->setParentItem(newLocItem_);

    aItem_->setHome(QPointF(0,0));

    aItem_->goHome();
    //aItem_->setPos(QPointF(0,0));
}
