#ifndef LOCATIONITEM_HH
#define LOCATIONITEM_HH

#include <QMainWindow>
#include <QObject>

#include "mapitem.hh"
#include "commonresource.hh"
#include "agent.hh"
#include "influence.h"
#include "../Course/random.h"
#include <cmath>

/**
 * @file
 * @brief Locationitem is the graphic representation of a location on map
 */

class agentItem;

class LocationItem : public mapItem
{
    Q_OBJECT
public:
    LocationItem(const std::shared_ptr<Interface::Location> location, std::vector<std::pair<QString, QString>> spritePaths, std::shared_ptr<Interface::CommonResource> localRes, std::shared_ptr<Interface::CommonResource> demandRes);
    ~LocationItem();

    /**
     * @brief Creates a shape for drawable objects (a rectangle)
     * @return Returns a drawable square item
     */
    QRectF boundingRect() const override;

    /**
     * @brief Draws the square items, their sprites and their names
     * @param painter
     * @param option
     * @param widget
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    /**
     * @brief Method triggered by clicking a LocationItem on scene
     * @param event
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    /**
     * @brief hoverEnterEvent
     * @param event
     */
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override; // Käytössä?
    /**
     * @brief hoverLeaveEvent
     * @param event
     */
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override; // Käytössä?

    /**
     * @brief Returns the base class of LocationItem
     * @return Returns a shared pointer to base class Interface::Location
     */
    const std::shared_ptr<Interface::Location> getObject();

    /**
     * @brief Returns the base value for each location used for resource distribution
     * @return
     */
    int getBasevalue();

    /**
     * @brief Test to see if an item is a LocationItem
     * @return Returns a string of it's type's name
     */

    const QString typeOf() override{return "locationitem";} // KÄytössä?

    /**
     * @brief Accepts agent as its child
     * @param aItem: AgentItem incoming to LocationItem
     *
     */
    void acceptAgent(agentItem* aItem);

    /**
     * @brief Calculate how much resource rewards a player can receive after round
     * @param player: Player in question (who clicked on location)
     * @return Returns a vector consisting of calculated sum of receivable rewards, amount of player's own agents and rivaling agents in location.
     * @pre Location has base value and resources to distribute
     * @post Numbers are displayed on Popup Dialog displayed when clicked on a LocationItem
     */
    std::vector<int> calculateRewards(std::shared_ptr<Interface::Player> &player);

    // Methods for location's local resource
    /**
     * @brief Get the locations local resource
     * @return Return a pointer to the local resource
     */
    std::shared_ptr<Interface::CommonResource> getLocalResource();

    // Methods for location's demanded resources
    /**
     * @brief Sets a demanded resource for a location. Used for generating new resource demands.
     * @param res: Resource to be set for demand
     * @pre New resource demand is generated properly and not the same as local resource
     * @post New demand is displayed on Popup window after one demand gets completed
     */
    void setDemandedResource(std::shared_ptr<Interface::CommonResource> &res);
    /**
     * @brief Get the demanded resource of a locations
     * @return Returns a pointer to a demanded resource in location
     */
    std::shared_ptr<Interface::CommonResource> getDemandedResource();
    
    /**
     * @brief Generates a new demand for location to be completed.
     * @pre Old demand is completed. New demanded resource is not the same as the local collectible resource.
     * @post New demand is set for location and displayed on popup dialog
     */
    void generateNewDemand();

    /**
     * @brief rearrange
     */
    void rearrange() override;

    /**
     * @brief setNeighbours
     * @param neighbours
     */
    void setNeighbours(std::pair<LocationItem*, LocationItem*> neighbours){neighbours_=neighbours;}
    std::pair<LocationItem*, LocationItem*> neighbours(){return neighbours_;}

    /**
     * @brief Get the derived class AgentItem pointer for an AgentInterface object
     * @param agent: Agent whose graphical item is needed
     * @return Agent's corresponding AgentItem
     */
    agentItem *getAgentItemFor(std::shared_ptr<Interface::AgentInterface> agent);

    QPixmap* governorPixmap(){return governorImage_;}
    QPixmap* resourcePixmap(){return resourceImage_;}


signals:
    void locationItemPressed(LocationItem*);

private:
    const std::shared_ptr<Interface::Location> locationObject_;
    int basevalue_ = 0;
    bool isSelected = false;
    bool isHovered_ = false;

    // Resources are initially set to a constant NULL to avoid errors
    std::shared_ptr<Interface::CommonResource> localRes_ = nullptr;
    std::shared_ptr<Interface::CommonResource> demandRes_ = nullptr;

    std::pair<LocationItem*, LocationItem*> neighbours_;

    QPixmap* governorImage_;
    QPixmap* planetImage_;
    QPixmap* resourceImage_;

};

#endif // LOCATIONITEM_HH
