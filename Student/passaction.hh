#ifndef PASSACTION_HH
#define PASSACTION_HH
#include "agentactioninterface.hh"
#include "playerhand.hh"

class PassAction : public AgentActionInterface
{
public:
    // Passing equals discarding all your action cards. If you don't like moving, you can just press the pass button!
    PassAction(PlayerHand* hand);
    // returns always true
    virtual bool canPerform() const override;
    //
    virtual void perform() override;
    // No target so nullptr
    virtual mapItem *getTargetMapItem() override;

    virtual QString pastTenseDescription() override;
private:
    std::shared_ptr<Interface::Player> player_;
    PlayerHand* hand_;
};

#endif // PASSACTION_HH