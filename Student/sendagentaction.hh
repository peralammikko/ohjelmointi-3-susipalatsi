#ifndef SENDAGENTACTION_HH
#define SENDAGENTACTION_HH

#include "../Course/actioninterface.h"
#include <memory>


class LocationItem;
class agentItem;

class SendAgentAction : public Interface::ActionInterface
{
public:
    SendAgentAction(LocationItem *newLocItem, agentItem *aItem);
    ~SendAgentAction();

    virtual bool canPerform() const override;

    virtual void perform() override;
private:
    LocationItem* newLocItem_;
    LocationItem* oldLocItem_;
    agentItem* aItem_;

};

#endif // SENDAGENTACTION_HH
