#ifndef AGENTCARD_HH
#define AGENTCARD_HH

#include "agentinterface.h"
#include "location.h"
#include "cardinterface.h"

class agentCard : public Interface::AgentInterface
{
public:
    explicit agentCard(std::shared_ptr<Interface::AgentInterface>);


};

#endif // AGENTCARD_HH
