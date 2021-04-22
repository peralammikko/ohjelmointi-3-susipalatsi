#ifndef AGENTACTIONINTERFACE_HH
#define AGENTACTIONINTERFACE_HH
#include "actioninterface.h"
#include "mapitem.hh"

class mapItem;
class AgentActionInterface : public Interface::ActionInterface
{
public:
    AgentActionInterface();
    virtual mapItem* getTargetMapItem(){return nullptr;}
    virtual QString pastTenseDescription()=0;
};

#endif // AGENTACTIONINTERFACE_HH
