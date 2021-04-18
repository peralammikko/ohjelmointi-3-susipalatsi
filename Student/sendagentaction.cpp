#include "sendagentaction.hh"
#include "agentitem.hh"
#include "locationitem.hh"


SendAgentAction::SendAgentAction(LocationItem* newLocItem, agentItem* aItem) : newLocItem_(newLocItem), aItem_(aItem)
{

}

SendAgentAction::~SendAgentAction()
{

}

bool SendAgentAction::canPerform() const
{
    auto oldplacement = dynamic_cast<LocationItem*>(aItem_->parentItem());
    if (!oldplacement)
    {
        // TODO: require payment
        // qDebug() << "attempting move an agent from hand to place";
       return true;
    } else {
       // oldPlacInterface->removeAgent(aInterface);
        // qDebug() << "attempting move an agent from place to place";
        // Calculate the distances between locations
        int dist = abs(newLocItem_->mapIndex() - oldplacement->mapIndex());
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
    std::shared_ptr<Interface::AgentInterface> aInterface = aItem_->getObject();
    std::shared_ptr<Interface::Location> newPlacInterface = newLocItem_->getObject();
    std::shared_ptr<Interface::Location> oldPlacInterface = aInterface->placement().lock();

    // Removes agent from its previous location, sends the agent to new location and sets new "home coords"
    if (oldPlacInterface){
        oldPlacInterface->removeAgent(aInterface);
    }
    newPlacInterface->sendAgent(aInterface);
    aItem_->setParentItem(newLocItem_);
    aItem_->setHome(QPointF(0,0));
    aItem_->goHome();
}

/*

  Old action here

bool GameScene::canMoveAgent()
{
    if (!game_.lock())
    {
        //todo: throw a real error
        qDebug() << "Fatal error!";
        return false;
    }
    std::shared_ptr<Interface::AgentInterface> aInterface = aItem->getObject();
    std::shared_ptr<Interface::Location> newPlacInterface = newLocItem->getObject();
    std::shared_ptr<Interface::Location> oldPlacInterface = aInterface->placement().lock();
    if (!oldPlacInterface)
    {
       // TODO: emit some signal to check viability
       //
       return true;
    } else {
       // oldPlacInterface->removeAgent(aInterface);

        auto locs = game_.lock()->locations();
        // get itarator of the agent and the itarator of the targeted location in game's locvec
        auto targetIt = std::find(locs.begin(), locs.end(), newPlacInterface);
        auto startingIt = std::find(locs.begin(), locs.end(), oldPlacInterface);
        if (startingIt != locs.end() and targetIt != locs.end())
        {
              // Calculate the distances between locations
              long dist = abs(std::distance(startingIt, targetIt));
              // TODO: maybe implement movements which are larger than one
              if ( dist == 1 or dist == locs.size()-1 )
              {
                  // TODO: maybe additional pooling
                  return true;
              }
        }
    }
    return false;
}
*/

/*
void GameScene::moveAgent(LocationItem* newLocItem, agentItem* aItem)
{
    std::shared_ptr<Interface::AgentInterface> aInterface = aItem->getObject();
    std::shared_ptr<Interface::Location> newPlacInterface = newLocItem->getObject();
    std::shared_ptr<Interface::Location> oldPlacInterface = aInterface->placement().lock();

    // Removes agent from its previous location, sends the agent to new location and sets new "home coords"
    if (oldPlacInterface){
        oldPlacInterface->removeAgent(aInterface);
    }
    newPlacInterface->sendAgent(aInterface);
    aItem->setParentItem(newLocItem);
    aItem->setHome(QPointF(0,0));
}
*/
