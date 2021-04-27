#ifndef AGENT_HH
#define AGENT_HH
#include "../Course/agentinterface.h"
#include "../Course/player.h"

#include <memory>
#include <map>
#include <deque>

#include "../Course/agentinterface.h"
#include "commonresource.hh"
#include "locationitem.hh"
#include "../Course/councilor.h"

class LocationItem;

namespace Interface {
/**
 * @file
 * @brief own Agent class for agent related data
 */
class Agent : public AgentInterface
{
public:
    Agent(QString name, std::weak_ptr<Player> owner);
    ~Agent();

    /**
     * @brief Initializes the agent's resource map for each location and it's corresponding local resource
     * @param agentResMap: An empty resourcemap for each agent consisting of locations and their resources
     * @pre Agent exists and initial resource maps have been created
     */
    void initAgentResources(AgentResourceMap agentResMap);
    /**
     * @brief Get the gathered resources of this agent
     * @return Return an agent's inventory of resources collected per location
     */
    AgentResourceMap getAgentResources();
    /**
     * @brief Adds resources to an agent
     * @param agentAt: Location where agent is residing and collecting resources
     * @param res: Resource to be added to agent
     * @param amount: How much resource agent is getting
     * @pre Agent is in a location to be collecting resources
     * @post Resources get added to the agents inventory
     */
    void addResource(std::shared_ptr<Location> agentAt, std::shared_ptr<CommonResource> res, int amount);
    /**
     * @brief Removes resources from agents inventory. Used in trading resources for councilor cards
     * @param agentAt: Where current agent is making the trade
     * @param amount: How many resources need to be deleted
     * @pre Agent is in a location and made a successful trade for a councilor card
     * @post Resources are removed and agent gaing a councilor card
     */
    void removeResource(std::shared_ptr<Location> &agentAt, int amount);
    /**
     * @brief Attempts to give the agent a councilor card after a successful trade
     * @param card: Whose councilor card is being given
     * @return If agent has an empty card holder, returns true. Otherwise returns false.
     *
     */
    bool addCouncilCard(std::shared_ptr<Councilor> card);
    /**
     * @brief Check if agent is holding a councilor card.
     * @return True if card holder is empty. False if not.
     */
    bool hasCouncilCard();
    /**
     * @brief Gets the pointer to the councilor card an agent is holding.
     * @return Pointer to councilor.
     */
    std::shared_ptr<Councilor> getCouncilCard();

    /**
     * @brief Locates the agent
     * @return Return a pointer to the location an agent is now placed in
     */
    std::shared_ptr<Interface::Location> whereIsAgent();

    // AgentInterface overrides
    virtual bool isCommon() const override;
    virtual std::weak_ptr<Location> placement() const override;
    virtual void setPlacement(std::weak_ptr<Location> placement) override;
    virtual unsigned short connections() const  override;
    virtual void setConnections(unsigned short connections) override;
    virtual void modifyConnections(short change) override;

    // CardInterface overrides
    virtual QString typeName() const override;
    virtual QString name() const override;
    virtual QString title() const override;
    virtual std::weak_ptr<Player> owner() const override;
    virtual void setOwner(std::weak_ptr<Player> owner) override;
    // for our sakes and purposes this method is the same as placement()
    virtual std::weak_ptr<Location> location() const override;

private:
    QString name_;
    std::weak_ptr<Player> owner_;
    std::weak_ptr<Location> placement_;
    QString title_;

    AgentResourceMap gatheredResources_;
    std::shared_ptr<Councilor> councilCardHolder = nullptr;
};


} // Interface
#endif // AGENT_HH
