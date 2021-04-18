#ifndef WITHDRAWAGENTACTION_HH
#define WITHDRAWAGENTACTION_HH

#include "../Course/actioninterface.h"

class WithdrawAgentAction : Interface::ActionInterface
{
public:
    WithdrawAgentAction();

    virtual bool canPerform() const override;

    virtual void perform() override;
};

#endif // WITHDRAWAGENTACTION_HH
