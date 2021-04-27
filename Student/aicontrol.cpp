#include "aicontrol.hh"
#include <algorithm>

namespace Interface {


AiControl::AiControl(GameScene *gameScene, std::shared_ptr<Player> player) : gameScene_(gameScene), player_(player)
{
    hand_ =  gameScene_->playerHands().at(player_);
}

std::shared_ptr<ActionInterface> AiControl::nextAction()
{
     action_ = nullptr;
     auto actioncards = hand_->getCardItems();
     Q_ASSERT(actioncards.size() > 0);
     ponderActions();
     if (!action_.get()){
         return  std::make_shared<PassAction>(hand_);
     } else {
         auto paymentcard = actioncards.at(0);
         player_->playCard(paymentcard->getCard());
         auto location = paymentcard->getCard()->location().lock();
         if (location) {
             location->discards().get()->addCard(paymentcard->getCard());
         }
         delete paymentcard;
         hand_->rearrange();
         qDebug() << "12342423";
         return action_;
     }
}

void AiControl::ponderActions()
{
    Q_ASSERT(gameScene_ != nullptr);
    Q_ASSERT(hand_ != nullptr);
    hand_ = gameScene_->playerHands().at(player_);
    std::shared_ptr<ActionInterface> action;

    auto handAgents = hand_->getAgentItems();
    locItems_ = gameScene_->GetLocItems();
    // These aitems are in locations
    aitems_ =  gameScene_->getAgentItemsForPlayer(player_);
    for (unsigned int i = 0; i < considerationsVector.size(); ++i){
         if (action_.get() == nullptr){
            (this->*considerationsVector[i])();
         } else {
             return;
         }
    }

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
    int reqAmount = neededRes->amount();
    int agentHas = 0;
    std::shared_ptr<Interface::Agent> agent = aitem->getAgentClass();
    auto agentResources = agent->getAgentResources();
    std::shared_ptr<Interface::Location> demandLoc = neededRes->location().lock();

    if (demandLoc) {
        agentHas = agentResources.at(demandLoc).size();
    }
    if (agentHas >= reqAmount) {
        return true;
    } else {
        return false;
    }
}

int AiControl::considerSendingFromHand()
{
    Q_ASSERT(gameScene_ != nullptr);
    Q_ASSERT(hand_ != nullptr);
    auto handAgents = hand_->getAgentItems();
    auto locItems = gameScene_->GetLocItems();
    // These aitems are in locations
    std::vector<agentItem*> aitems =  gameScene_->getAgentItemsForPlayer(player_);
    // The AI prioritizes playing agents from its hand
    if (handAgents.size()){
        unsigned int randomLocIndex = Interface::Random::RANDOM.uint(locItems.size()-1);
        auto ac = std::make_shared<SendAgentAction>(locItems.at(randomLocIndex), handAgents.at(0));
        if (ac->canPerform()){
            action_ = ac;
            return 1;
        }
    }
    return 1;
}

int AiControl::considerWithdrawing()
{
    Q_ASSERT(hand_ != nullptr);
    for (auto aitem : aitems_){
        auto agent = aitem->getAgentClass();
        auto playercards = player_->cards();
        // find the locitem where the aitem is
        LocationItem* homeItem = findHomeItemFor(aitem);
        if (!homeItem){
            continue;
        }
        if (aitem->getAgentClass()->hasCouncilCard()) {
            // The way how councilcards work is that the agent keeps on carrying it, apparently.
            // To prevent returning the card multiple times, check if the player already has that card.
            auto councilcardFind = std::find(playercards.begin(), playercards.end(), aitem->getAgentClass()->getCouncilCard());
            if (councilcardFind == playercards.end()){
                auto ac = std::make_shared<WithdrawAgentAction>(hand_, aitem);
                if (ac->canPerform()){
                    action_ = ac;
                    return 1;
                }
            }
        }
    }
}

int AiControl::considerSmartMoving()
{
    for (auto aItem : aitems_){
        auto homeItem = findHomeItemFor(aItem);
        Q_ASSERT(homeItem != nullptr);
        // Moving to a location in which Councilor demands items which the agent has is on third highest priority
        auto neighbours = homeItem->neighbours();
        if (canGetCounilorCard(aItem, neighbours.first)){
            auto ac = std::make_shared<SendAgentAction>(neighbours.first, aItem);
            if (ac->canPerform()){
                action_ = ac;
                return 1;
            }
        }
        if (canGetCounilorCard(aItem, neighbours.second)){
            auto ac = std::make_shared<SendAgentAction>(neighbours.second, aItem);
            if (ac->canPerform()){
                action_ = ac;
                return 1;
            }
        }
    }
    return 1;
}

int AiControl::considerRandomMoving()
{
    for (auto aItem : aitems_){
        auto homeItem = findHomeItemFor(aItem);
        Q_ASSERT(homeItem != nullptr);
        auto neighbours = homeItem->neighbours();
        int influence = homeItem->getObject()->influence(player_);
        unsigned int rand = Interface::Random::RANDOM.uint(std::max(10-influence, 5));
        if (rand == 0){
            auto ac = std::make_shared<SendAgentAction>(neighbours.first, aItem);
            if (ac->canPerform()){
                action_ = ac;
                return 1;
            }
        } else if (rand == 1) {
            auto ac = std::make_shared<SendAgentAction>(neighbours.second, aItem);
            if (ac->canPerform()){
                action_ = ac;
                return 1;
            }
        }
    }
    return 1;
}

LocationItem* AiControl::findHomeItemFor(agentItem *aItem)
{
    for (auto locItem : locItems_){
        if (aItem->parentItem() == locItem){
            return locItem;
        }
    }
    return nullptr;
}

void AiControl::awardCouncilorCard(agentItem *aitem, LocationItem *locItem)
{
    qDebug() << "GETTING";
    auto neededRes = locItem->getDemandedResource();
    int reqAmount = neededRes->amount();

    std::shared_ptr<Interface::Agent> agent = aitem->getAgentClass();
    auto agentResources = agent->getAgentResources();
    std::shared_ptr<Interface::Location> demandLoc = neededRes->location().lock();
    Q_ASSERT(demandLoc);

    aitem->getAgentClass()->addCouncilCard(locItem->getObject()->councilor());
    aitem->getAgentClass()->removeResource(demandLoc, reqAmount);


}

int AiControl::seeIfCanGetCouncilorCard()
{
    for (auto aitem : aitems_){
        LocationItem* homeItem = findHomeItemFor(aitem);
        if (!homeItem){
            continue;
        } else if (canGetCounilorCard(aitem, homeItem)){
            awardCouncilorCard(aitem, homeItem);
        }
    }
    return 1;
}

} // Interface
