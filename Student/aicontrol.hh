#ifndef AICONTROL_HH
#define AICONTROL_HH
#include "../Course/controlinterface.h"
#include "sendagentaction.hh"
#include "passaction.hh"
#include "gamescene.hh"
#include "random.h"
#include "withdrawagentaction.hh"

namespace Interface {



class AiControl : public ControlInterface
{
public:

    explicit AiControl(GameScene* gameScene, std::shared_ptr<Player> player);

    virtual ~AiControl() = default;

    virtual std::shared_ptr<ActionInterface> nextAction() override;

    void setNextAction(std::shared_ptr<ActionInterface> action);

private:
    // Calculates or randomizes a move for one of its agents. By default it returns a pass action.
    // If any of the agents can get a Councilor card in its location item, it also gathers that card.
    std::shared_ptr<ActionInterface> ponderActions();

    // Checks if one of AI's agents is legible to get a councilor card in its current placement.
    bool canGetCounilorCard(agentItem *aitem, LocationItem* locItem);

    // Pre: agent can get councilor card from location and is in that location
    // Post: agent has councilor card of locItem
    void getCouncilorCard(agentItem *aitem, LocationItem* locItem);

    std::shared_ptr<ActionInterface> action_;
    GameScene* gameScene_;
    std::shared_ptr<Player> player_;
};
} // Interface
#endif // AICONTROL_HH
