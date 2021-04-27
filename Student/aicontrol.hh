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
    /**
     * @brief AiControl a fairly smart AI control interface for robot players
     * @param gameScene
     * @param player
     */
    explicit AiControl(GameScene* gameScene, std::shared_ptr<Player> player);

    virtual ~AiControl() = default;

    virtual std::shared_ptr<ActionInterface> nextAction() override;

    void setNextAction(std::shared_ptr<ActionInterface> action);

private:
    std::shared_ptr<ActionInterface> action_;
    GameScene* gameScene_;
    std::shared_ptr<Player> player_;
    PlayerHand* hand_;
    std::vector<agentItem*> aitems_;
    std::vector<LocationItem*> locItems_;

    /**
     * @brief Goes through a list of actions and sees if any of them are possible and good game moves
     */
    void ponderActions();

    /**
     * @brief canGetCounilorCard sees if the agent in locItem fills requisites for gatherin councilor cad
     * @param aitem
     * @param locItem
     * @return
     */
    bool canGetCounilorCard(agentItem *aitem, LocationItem* locItem);

    /**
     * @brief findHomeItemFor tries to find locationitem in which agent resides
     * @param aItem
     * @return the locationitem where agent is or nullptr
     */
    LocationItem* findHomeItemFor(agentItem* aItem);

    /**
     * @brief awardCouncilorCard
     * @pre gent can get councilor card from location (has enough of the needed resources)
     * @param aitem
     * @param locItem
     * @post agent has councilor card of locItem
     */
    void awardCouncilorCard(agentItem *aitem, LocationItem* locItem);

    // Member functions ending in int are pondering functions

    /**
     * @brief seeIfCanGetCouncilorCard First in line of actions. If there are possible councilor cards to be harvested (same as in the loc pop-up), the AI takes it.
     * @return
     */
    int seeIfCanGetCouncilorCard();
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
            &AiControl::seeIfCanGetCouncilorCard,
            &AiControl::considerSendingFromHand,
            &AiControl::considerWithdrawing,
            &AiControl::considerSmartMoving,
            &AiControl::considerRandomMoving
    };





};
} // Interface
#endif // AICONTROL_HH
