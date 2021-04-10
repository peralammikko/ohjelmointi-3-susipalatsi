#ifndef PLAYERCLASS_HH
#define PLAYERCLASS_HH

#include "agentitem.hh"
#include "player.h"

class Playerclass
{
public:
    Playerclass(std::shared_ptr<Interface::Player> &player);
    void spawnAgent();
    std::vector<agentItem *> getAgents();
    agentItem *sendAgent();

private:
    const std::shared_ptr<Interface::Player> playerObject_;
    std::vector<agentItem*> playerAgents_;
};

#endif // PLAYER_HH
