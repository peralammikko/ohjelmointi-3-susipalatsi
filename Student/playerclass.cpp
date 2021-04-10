#include "playerclass.hh"


Playerclass::Playerclass(std::shared_ptr<Interface::Player> &player) : playerObject_(player)
{

}

void Playerclass::spawnAgent()
{
    std::shared_ptr<Interface::AgentInterface> agentptr = nullptr;
    agentItem* agentti = new agentItem(agentptr);
    playerAgents_.push_back(agentti);
}

std::vector<agentItem*> Playerclass::getAgents()
{
    return playerAgents_;
}

agentItem* Playerclass::sendAgent()
{
    if (playerAgents_.size() != 0) {
        agentItem* last = playerAgents_.back();
        playerAgents_.pop_back();
        return last;
    }
    return nullptr;
}
