#include "cardpaymentaction.hh"

CardPaymentAction::CardPaymentAction(agentItem *aItem) : aItem_(aItem)
{

}

bool CardPaymentAction::canPerform() const
{
    return aItem_->isWaitingForAction();
}

void CardPaymentAction::perform()
{
    return;
}
