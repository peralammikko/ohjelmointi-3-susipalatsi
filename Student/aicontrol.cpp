#include "aicontrol.hh"

namespace Interface {


AiControl::AiControl(GameScene *gameScene, std::shared_ptr<Player> player) : gameScene_(gameScene), player_(player)
{

}

std::shared_ptr<ActionInterface> AiControl::nextAction()
{
     auto hand = gameScene_->playerHands().at(player_);
     auto actioncards = hand->getCardItems();

     Q_ASSERT(actioncards.size() > 0);
     std::shared_ptr<ActionInterface> action = ponderActions();
     if (!action.get()){
         return  std::make_shared<PassAction>(hand);
     } else {
         auto paymentcard = actioncards.at(0);
         player_->playCard(paymentcard->getCard());
         auto location = paymentcard->getCard()->location().lock();
         if (location) {
             location->discards().get()->addCard(paymentcard->getCard());
         }
         delete paymentcard;
         hand->rearrange();
         return action;
     }
}

std::shared_ptr<ActionInterface> AiControl::ponderActions()
{
    Q_ASSERT(gameScene_ != nullptr);
    auto hand = gameScene_->playerHands().at(player_);
    std::shared_ptr<ActionInterface> action;

    auto handAgents = hand->getAgentItems();
    auto locItems = gameScene_->GetLocItems();
    // These aitems are in locations
    std::vector<agentItem*> aitems =  gameScene_->getAgentItemsForPlayer(player_);

    // The AI prioritizes playing agents from its hand
    if (handAgents.size()){
        unsigned int randomLocIndex = Interface::Random::RANDOM.uint(locItems.size()-1);
        action = std::make_shared<SendAgentAction>(locItems.at(randomLocIndex), handAgents.at(0));
        if (action->canPerform()){
            return action;
        }
    }
    for (auto aitem : aitems){
        auto agent = aitem->getAgentClass();
        auto playercards = player_->cards();
        // find the locitem where the aitem is
        LocationItem* homeItem = nullptr;
        for (auto locItem : locItems){
            if (aitem->parentItem() == locItem){
                homeItem = locItem;
                break;
            }
        }
        if (!homeItem){
            continue;
        }
        // Check if agent can get councilor card in its home location
        // TODO: add influence to the check
        if (canGetCounilorCard(aitem, homeItem)){
            getCouncilorCard(aitem, homeItem);
        }

        // Returning councilor cards takes second highest priority
        if (aitem->getAgentClass()->hasCouncilCard()) {
            // The way how councilcards work is that the agent keeps on carrying it, apparently.
            // To prevent returning the card multiple times, check if the player already has that card.
            auto councilcardFind = std::find(playercards.begin(), playercards.end(), aitem->getAgentClass()->getCouncilCard());
            if (councilcardFind == playercards.end()){
                action = std::make_shared<WithdrawAgentAction>(hand, aitem);
                if (action->canPerform()){
                    return action;
                }
            }
        }
        // Moving to a location in which Councilor demands items which the agent has is on third highest priority
        auto neighbours = homeItem->neighbours();
        if (canGetCounilorCard(aitem, neighbours.first)){
            action = std::make_shared<SendAgentAction>(neighbours.first, aitem);
            if (action->canPerform()){
                return action;
            }
        }
        if (canGetCounilorCard(aitem, neighbours.second)){
            action = std::make_shared<SendAgentAction>(neighbours.second, aitem);
            if (action->canPerform()){
                return action;
            }
        }

        // Agent has a small chance to be moved if the AI already has sufficient influence in the area
        if (homeItem->getObject()->influence(player_) > 4){
            unsigned int rand = Interface::Random::RANDOM.uint(6);
            if (rand == 0){
                action = std::make_shared<SendAgentAction>(neighbours.first, aitem);
                if (action->canPerform()){
                    return action;
                }
            } else if (rand == 1) {
                if (canGetCounilorCard(aitem, neighbours.second)){
                    action = std::make_shared<SendAgentAction>(neighbours.second, aitem);
                    if (action->canPerform()){
                        return action;
                    }
                }
            }
        }
    }

    return action;
}

bool AiControl::canGetCounilorCard(agentItem *aitem, LocationItem *locItem)
{
    if (aitem->getAgentClass()->hasCouncilCard()){
        return false;
    }
    if (locItem->getObject()->influence(player_) < 5){
        return false;
    } else {
        qDebug() << "AI has agent with 5 or more influence in a location";
    }

    auto neededRes = locItem->getDemandedResource();
    int reqAmount = neededRes.amount();
    int agentHas = 0;
    std::shared_ptr<Interface::Agent> agent = aitem->getAgentClass();
    auto agentResources = agent->getAgentResources();
    std::shared_ptr<Interface::Location> demandLoc = neededRes.location().lock();

    if (demandLoc) {
        agentHas = agentResources.at(demandLoc).size();
    }
    if (agentHas >= reqAmount) {
        return true;
    } else {
        return false;
    }
}

 void AiControl::getCouncilorCard(agentItem *aitem, LocationItem *locItem)
{
    auto neededRes = locItem->getDemandedResource();
    int reqAmount = neededRes.amount();

    std::shared_ptr<Interface::Agent> agent = aitem->getAgentClass();
    auto agentResources = agent->getAgentResources();
    std::shared_ptr<Interface::Location> demandLoc = neededRes.location().lock();
    Q_ASSERT(demandLoc);

    aitem->getAgentClass()->addCouncilCard(locItem->getObject()->councilor());
    aitem->getAgentClass()->removeResource(demandLoc, reqAmount);
    // TODO: emit signal to history

}

} // Interface
