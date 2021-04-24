#include "sendagentaction.hh"
#include "agentitem.hh"
#include "locationitem.hh"


SendAgentAction::SendAgentAction(LocationItem* newLocItem, agentItem* aItem) : newLocItem_(newLocItem), aItem_(aItem)
{
    oldLocInterface_ = aItem_->getAgentClass()->placement().lock();
}

SendAgentAction::~SendAgentAction(){}

bool SendAgentAction::canPerform() const
{
    auto oldLocItem = dynamic_cast<LocationItem*>(aItem_->parentItem());
    if (!oldLocItem)
    {
       return true;
    } else if (oldLocItem != newLocItem_){
        auto neighbours = oldLocItem->neighbours();
        if (neighbours.first == newLocItem_ or neighbours.second == newLocItem_){
            return true;
        }
    }
    return false;
}

void SendAgentAction::perform()
{
    std::shared_ptr<Interface::Agent> aInterface = aItem_->getAgentClass();
    std::shared_ptr<Interface::Location> newPlacInterface = newLocItem_->getObject();
    std::shared_ptr<Interface::Location> oldPlacInterface = aInterface->placement().lock();

    if (oldPlacInterface){
        oldPlacInterface->removeAgent(aInterface);
    }
    newPlacInterface->sendAgent(aInterface);

    // Convert current position on scene to new parent item's coordinates
    // Do this so that the item does not fly far far away
    QPointF currentPos = aItem_->scenePos();
    aItem_->setPos(newLocItem_->mapFromScene(currentPos));
    aItem_->setParentItem(newLocItem_);
    newLocItem_->rearrange();
   // aItem_->setHome(QPointF(0,0));
   // aItem_->goHome();
}

mapItem *SendAgentAction::getTargetMapItem()
{
    return newLocItem_;
}

QString SendAgentAction::pastTenseDescription()
{
    QString description = "sent " + aItem_->getAgentClass()->name();
    if (oldLocInterface_){
        description += " from " + oldLocInterface_->name();
    }
    description += " to " + newLocItem_->getObject()->name();
    return description;
}
