#ifndef CARDPAYMENTACTION_HH
#define CARDPAYMENTACTION_HH
#include "actioninterface.h"
#include "agentitem.hh"

class CardPaymentAction : public Interface::ActionInterface
{
public:
    // This class is for action when players are prompted to pay an action card on their agent
    CardPaymentAction(agentItem* aItem);

    virtual bool canPerform() const override;

    virtual void perform() override;
private:
    agentItem *aItem_;
};

#endif // CARDPAYMENTACTION_HH
