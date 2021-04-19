#ifndef WITHDRAWAGENTACTION_HH
#define WITHDRAWAGENTACTION_HH

#include "../Course/actioninterface.h"

class PlayerHand;
class agentItem;


class WithdrawAgentAction : public Interface::ActionInterface
{
public:
    // Withdrawing an agent back in hand from a location
    WithdrawAgentAction(PlayerHand *hand, agentItem *aItem);
    // Agent is not in hand and the hand graphicsitem belongs to the same player as the agent
    virtual bool canPerform() const override;
    // Moves the agent to player hand. agentItem's parent is now playerhand graphicsitem
    virtual void perform() override;
private:
    PlayerHand* hand_;
    agentItem* aItem_;
};

#endif // WITHDRAWAGENTACTION_HH
