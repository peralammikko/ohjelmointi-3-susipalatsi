#ifndef CARDPAYMENTACTION_HH
#define CARDPAYMENTACTION_HH
#include "agentactioninterface.hh"
#include "agentitem.hh"

/**
 * @file
 * @brief a pseudo-action for paying cards
 */
class CardPaymentAction : public AgentActionInterface
{
public:
    /**
     * @brief CardPaymentAction This class is for action when players are prompted to pay an action card on their agent
     * @param aItem
     */
    CardPaymentAction(agentItem* aItem);

    virtual bool canPerform() const override;

    virtual void perform() override;

    virtual mapItem * getTargetMapItem() override {return nullptr;}

    virtual QString pastTenseDescription() override;
private:
    agentItem *aItem_;
};

#endif // CARDPAYMENTACTION_HH
