#include "resourcedealer.hh"
#include "stateexception.h"

ResourceDealer::ResourceDealer(GameScene *gameScene, std::shared_ptr<Interface::Game> game) : gameScene_(gameScene), game_(game)
{

}

void ResourceDealer::onReadyForResources()
{
    rewardResources();
}


void ResourceDealer::rewardResources()
{
    std::vector<LocationItem*> items = gameScene_->GetLocItems();
    int resourceTotalAmount;
    agentItem* aItem;
    int rewardAmount = 1;
    for (auto player : game_->players()) {

        // Container for each unique location player has agents in (used to reward influence points)
        std::set<std::shared_ptr<Interface::Location>> locationsBeen = {};

        // Always give players one action card so they don't get stuck!
        std::shared_ptr<Interface::ActionCard> card = std::make_shared<Interface::ActionCard>();
        player->addCard(card);
        gameScene_->addActionCardForPlayer(player, card);

        for (auto card : player->cards()) {
            std::shared_ptr<Interface::Agent> agentPtr = std::dynamic_pointer_cast<Interface::Agent>(card);

            // If card is an agent card
            if (agentPtr) {
                std::shared_ptr<Interface::Location> agentAt = agentPtr->placement().lock();
                if (agentAt) {
                    // If agent is in any location (nullptr means no location a.k.a. "home"
                    if (agentAt != nullptr) {
                        resourceTotalAmount = 0;
                        QString resSpritePth = "";
                        locationsBeen.insert(agentAt);
                        std::shared_ptr<Interface::CommonResource> res = resMap_.at(agentAt);

                        // Find the correct location item for calculating rewards
                        for (auto loc : items)
                        {
                            if (loc->getObject()->id() == agentAt->id()) {
                                auto rewards = loc->calculateRewards(player);
                                rewardAmount = rewards.at(0);
                                aItem = loc->getAgentItemFor(agentPtr);
                                if (aItem == nullptr){   
                                    throw Interface::StateException(QString("Agent "+agentPtr->name() +" could not have its agent item located during a parliamentary day"));
                                }
                            }
                        }
                        for (int i = 0; i < rewardAmount; ++i)
                        {
                            if (agentAt->deck()->canDraw()){
                                auto drawnCard = agentAt->deck()->draw();
                                auto resu = std::dynamic_pointer_cast<Interface::CommonResource>(drawnCard);
                                if (resu)
                                {
                                    agentPtr->addResource(agentAt, res, resu->amount());
                                    resourceTotalAmount += resu->amount();
                                    resSpritePth = resu->getSpritePath();
                                    agentAt->discards()->addCard(drawnCard);
                                } else {
                                    auto action = std::dynamic_pointer_cast<Interface::ActionCard>(drawnCard);
                                    if (action) {
                                        auto owner = agentPtr->owner().lock();
                                        owner->addCard(action);
                                        gameScene_->addActionCardForPlayer(owner, action);
                                    } else {
                                        throw Interface::StateException(QString(("Agent "+agentPtr->name()
                                                                                 +" drew an unsupported type card "+ drawnCard->typeName()
                                                                                 +" in " + agentAt->name())));
                                    }
                                }
                            }
                        }
                        aItem->displayResourceChange(resourceTotalAmount, resSpritePth);
                        // Agents earning resources passively by staying at location. Keep or not?

                        // agentPtr->addResource(agentAt, res, rewardAmount);


                        ///////////// CHEAT CODE ///////////////
                        // agentPtr->addResource(agentAt, res, 100);
                        ////////////////////////////////////////

                    } else {
                        throw Interface::StateException(QString("Agent "+agentPtr->name() +" could not be found during a parliamentary day"));
                    }
                } else {
                    qDebug() << agentPtr->name() << " not in any location";
                }
            }
        }
        for (auto loc : game_->locations()) {

            /////// CHEAT CODE ///////////
            // loc->setInfluence(player, 50);
            /////////////////////////////

            if (locationsBeen.find(loc) != locationsBeen.end()) {
                int inf = loc->influence(player);
                if (inf < 5) {
                    loc->setInfluence(player, inf+1);
                }
            }
        }
    }
}

void ResourceDealer::infoResourceMaps(ResourceMap &rmap, ResourceMap &dmap)
{
    resMap_ = rmap;
    demandsMap_ = dmap;
}
