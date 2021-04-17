#ifndef SENDAGENTACTION_HH
#define SENDAGENTACTION_HH

#include "../Course/actioninterface.h"

class SendAgentAction : public Interface::ActionInterface
{
public:
    SendAgentAction();
    ~SendAgentAction();

    virtual bool canPerform() const override;

    virtual void perform() override;


};

#endif // SENDAGENTACTION_HH
