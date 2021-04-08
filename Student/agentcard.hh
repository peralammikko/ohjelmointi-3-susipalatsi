#ifndef AGENTCARD_HH
#define AGENTCARD_HH

#include "agentinterface.h"
#include "location.h"
#include "cardinterface.h"

class agentCard
{
public:
    explicit agentCard(std::shared_ptr<Interface::AgentInterface>);


private:
    std::shared_ptr<Interface::AgentInterface> agentObject_;
    QString agentName_;
    int value_;
    int negSkill_;
};

#endif // AGENTCARD_HH
