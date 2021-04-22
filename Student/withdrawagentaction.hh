#ifndef WITHDRAWAGENTACTION_HH
#define WITHDRAWAGENTACTION_HH

#include "agentactioninterface.hh"

#include "agentitem.hh"
#include "playerhand.hh"
#include "locationitem.hh"

class PlayerHand;
class agentItem;


class WithdrawAgentAction : public AgentActionInterface
{
public:
    // Withdrawing an agent back in hand from a location
    WithdrawAgentAction(PlayerHand *hand, agentItem *aItem);
    // Agent is not in hand and the hand graphicsitem belongs to the same player as the agent
    virtual bool canPerform() const override;
    // Moves the agent to player hand. agentItem's parent is now playerhand graphicsitem
    virtual void perform() override;

    virtual mapItem* getTargetMapItem() override;

    virtual QString pastTenseDescription() override;
private:
    PlayerHand* hand_;
    agentItem* aItem_;

};

#endif // WITHDRAWAGENTACTION_HH
