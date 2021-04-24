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
    typedef void (AiControl::*somefunc)();
public:

    explicit AiControl(GameScene* gameScene, std::shared_ptr<Player> player);

    virtual ~AiControl() = default;

    virtual std::shared_ptr<ActionInterface> nextAction() override;

    void setNextAction(std::shared_ptr<ActionInterface> action);

private:
    // Calculates or randomizes a move for one of its agents. By default it returns a pass action.
    // If any of the agents can get a Councilor card in its location item, it also gathers that card.
    void ponderActions();

    // Checks if one of AI's agents is legible to get a councilor card in its current placement.
    bool canGetCounilorCard(agentItem *aitem, LocationItem* locItem);


    LocationItem* findHomeItemFor(agentItem* aItem);

    // Pre: agent can get councilor card from location and is in that location
    // Post: agent has councilor card of locItem
    void getCouncilorCard(agentItem *aitem, LocationItem* locItem);

    std::shared_ptr<ActionInterface> action_;
    GameScene* gameScene_;
    std::shared_ptr<Player> player_;
    PlayerHand* hand_;
    std::vector<agentItem*> aitems_;
    std::vector<LocationItem*> locItems_;

    /**
     * @brief considerSendingFromHand tries to send an agent from hand to a random location
     * @pre -
     * @post if conditions are met,  action_ is set to sendagentaction (hand -> location)
     */
    int considerSendingFromHand();
    /**
     * @brief considerWithdrawing looks if there are any agents which have councilor cards
     * @pre -
     * @post if conditions are met, action_ is sent to withdrawaction
     */
    int considerWithdrawing();
    /**
     * @brief considerSmartMoving looks if there are any agents in locations which have adjacent locations with demands the agent can fulfill
     * @pre locations have neighbours
     * @post if conditions are met, sets action_ to sendagentaction (location -> location)
     */
    int considerSmartMoving();
    /**
     * @brief considerRandomMoving looks if there are any agents in locations, and rolls a chance to move based on their influence
     * @pre locations have neighbours
     * @post if conditions are met, sets action_ to sendagentaction (location -> location)
     */
    int considerRandomMoving();


    // Stores consideration functions. When the AI needs to generate a new action, it goes this in order (from first to last)
    // which means that you can modify action "priority" by changing its index
    std::vector< int (AiControl::*)()> considerationsVector = {
            &AiControl::considerSendingFromHand,
            &AiControl::considerWithdrawing,
            &AiControl::considerSmartMoving,
            &AiControl::considerRandomMoving
    };





};
} // Interface
#endif // AICONTROL_HH
