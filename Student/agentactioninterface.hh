#ifndef AGENTACTIONINTERFACE_HH
#define AGENTACTIONINTERFACE_HH
#include "actioninterface.h"
#include "mapitem.hh"

class mapItem;
class AgentActionInterface : public Interface::ActionInterface
{
public:
    /**
     * @brief AgentActionInterface this is the modified version of the course's Action interface.
     */
    AgentActionInterface() = default;
    /**
     * @brief getTargetMapItem Some of our actions have targets, which we wanted to store in actions.
     * @return  Returns 1 map item which the action targets, or a nullptr if it does not have one.
     */
    virtual mapItem* getTargetMapItem(){return nullptr;}
    /**
     * @brief pastTenseDescription a brief description of what just happened
     * @return returns a string which can be displayed on history list widget
     */
    virtual QString pastTenseDescription()=0;
};

#endif // AGENTACTIONINTERFACE_HH
