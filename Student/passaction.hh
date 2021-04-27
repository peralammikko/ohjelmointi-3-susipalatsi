#ifndef PASSACTION_HH
#define PASSACTION_HH
#include "agentactioninterface.hh"
#include "playerhand.hh"

class PassAction : public AgentActionInterface
{
public:
    /**
     * @brief PassAction is the action where player simply discardes their hand of action cards.
     * @param hand The HandItem where action cards visually reside
     */
    PassAction(PlayerHand* hand);
    /**
     * @brief canPerform
     * @return always returns true
     */
    virtual bool canPerform() const override;
    /**
     * @brief perform removes every action card from the player's hand.
     */
    virtual void perform() override;
    /**
     * @brief getTargetMapItem
     * @return nullptr as this action is not targeting anything
     */
    virtual mapItem *getTargetMapItem() override;

    /**
     * @brief pastTenseDescription description to history
     * @return a string rescribing the action
     */
    virtual QString pastTenseDescription() override;
private:
    /**
     * @brief player_ owner of the hand
     */
    std::shared_ptr<Interface::Player> player_;
    PlayerHand* hand_;
};

#endif // PASSACTION_HH
