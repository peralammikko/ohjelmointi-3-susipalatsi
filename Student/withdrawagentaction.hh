#ifndef WITHDRAWAGENTACTION_HH
#define WITHDRAWAGENTACTION_HH

#include "../Course/actioninterface.h"

class PlayerHand;
class agentItem;


class WithdrawAgentAction : public Interface::ActionInterface
{
public:
    WithdrawAgentAction(PlayerHand *hand, agentItem *aItem);

    virtual bool canPerform() const override;

    virtual void perform() override;
private:
    PlayerHand* hand_;
    agentItem* aItem_;
};

#endif // WITHDRAWAGENTACTION_HH
