#ifndef SENDAGENTACTION_HH
#define SENDAGENTACTION_HH

#include "agentactioninterface.hh"
#include <memory>

/**
 * @file
 * @brief a basic action for agents to move them to locations
 */
class LocationItem;
class agentItem;

class SendAgentAction : public AgentActionInterface
{
public:
    /**
     * @brief SendAgentAction Sending agent from either hand or from a location to a location
     * @param newLocItem The targeted location item of this action
     * @param aItem The agent item calling this action
     */
    SendAgentAction(LocationItem *newLocItem, agentItem *aItem);
    /**
     * @brief destructor
     */
    ~SendAgentAction();

    /**
     * @brief canPerform Sees where the agent is and where it target location item (newLocItem) is
     * @return true if agent in hand or in a location adjacent to the newLocItem
     */
    virtual bool canPerform() const override;

    /**
     * @brief perform Moves the agent to its target location item.
     * @post agent is now in a new location, and its parentItem has been changed to the correct GUI-item
     */
    virtual void perform() override;

    virtual mapItem* getTargetMapItem() override;
    virtual QString pastTenseDescription() override;

private:
    LocationItem* newLocItem_;
    std::shared_ptr<Interface::Location> oldLocInterface_;
    agentItem* aItem_;

};

#endif // SENDAGENTACTION_HH
