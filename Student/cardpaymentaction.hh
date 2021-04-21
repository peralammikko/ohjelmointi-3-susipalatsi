#ifndef CARDPAYMENTACTION_HH
#define CARDPAYMENTACTION_HH
#include "agentactioninterface.hh"
#include "agentitem.hh"

class CardPaymentAction : public AgentActionInterface
{
public:
    // This class is for action when players are prompted to pay an action card on their agent
    CardPaymentAction(agentItem* aItem);

    virtual bool canPerform() const override;

    virtual void perform() override;

    virtual mapItem * getTargetMapItem() override {return nullptr;}
private:
    agentItem *aItem_;
};

#endif // CARDPAYMENTACTION_HH
