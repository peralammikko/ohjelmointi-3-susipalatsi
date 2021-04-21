#ifndef LOCATIONITEM_HH
#define LOCATIONITEM_HH

#include <QMainWindow>
#include <QObject>

#include "mapitem.hh"
#include "commonresource.hh"
#include "agent.hh"
// #include "gamescene.hh"
#include "influence.h"
#include "../Course/random.h"
//#include "agentitem.hh"
#include <cmath>
>>>>>>> 8fa27fd2977f1a65f037c2019037d79922c2279a

class agentItem;

class LocationItem : public mapItem
{
    Q_OBJECT
public:
    LocationItem(const std::shared_ptr<Interface::Location> location, int mapIndex);
    ~LocationItem();
    // Luodaan itemille muoto (neliö)
    QRectF boundingRect() const override;

    // Piirretään neliöt ja nimet + maalataan jos valittuna
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    // Klikkauksesta tapahtuva metodi
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    // Haetaan itemin kantaluokka (Location)
    const std::shared_ptr<Interface::Location> getObject();

    int getBasevalue();

    int mapIndex();

    void setMapIndex(int newIndex);

    const QString typeOf() override;

    // Accepts agent as its child
    // Could change this to bool and return false if this agent is not welcome here
    void acceptAgent(agentItem* aItem);

    // Calculate how much resources one player gets after round
    std::vector<int> calculateRewards(std::shared_ptr<Interface::Player> &player);

    // Methods for location's local resource
    void setLocalResource(Interface::CommonResource &res);
    Interface::CommonResource getLocalResource();

    // Methods for location's demanded resources
    void setDemandedResource(Interface::CommonResource &res);
    Interface::CommonResource getDemandedResource();

    void checkCouncillorCard();
    
    void generateNewDemand();

    void addInfluence(std::shared_ptr<Interface::Player> &player);

    // bool giveCouncilCard(std::shared_ptr<Interface::Agent> &agent);

    void rearrange() override;

protected:
    void advance(int phase) override;

signals:
    void locationItemPressed(LocationItem*);
    void requestNewDemand(std::shared_ptr<Interface::Location> &loc);

private:
    const std::shared_ptr<Interface::Location> locationObject_;
    int basevalue_ = 0;
    bool isSelected = false;
    bool isHovered_ = false;

    // Resources are initially set to a constant NULL to avoid errors
    Interface::CommonResource localRes_ = NULLRES;
    Interface::CommonResource demandRes_ = NULLRES;
    
    std::map<std::shared_ptr<Interface::Player>, int> playerInfluence_;
    int mapIndex_;

};

#endif // LOCATIONITEM_HH
