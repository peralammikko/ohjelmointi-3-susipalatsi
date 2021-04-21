#ifndef SENDAGENTACTION_HH
#define SENDAGENTACTION_HH

#include "agentactioninterface.hh"
#include <memory>


class LocationItem;
class agentItem;

class SendAgentAction : public AgentActionInterface
{
public:
    SendAgentAction(LocationItem *newLocItem, agentItem *aItem);
    ~SendAgentAction();

    virtual bool canPerform() const override;

    virtual void perform() override;

    virtual mapItem* getTargetMapItem() override;
private:
    LocationItem* newLocItem_;
    LocationItem* oldLocItem_;
    agentItem* aItem_;

};

#endif // SENDAGENTACTION_HH
