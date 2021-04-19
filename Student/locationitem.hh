#ifndef LOCATIONITEM_HH
#define LOCATIONITEM_HH

#include <QMainWindow>
#include <QObject>

#include "mapitem.hh"
#include "commonresource.hh"
#include "agent.hh"

class agentItem;

class LocationItem : public mapItem
{
    Q_OBJECT
public:
    LocationItem(const std::shared_ptr<Interface::Location> location, int mapIndex);

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

protected:
    void advance(int phase) override;

signals:
    void locationItemPressed(LocationItem*);

private:
    const std::shared_ptr<Interface::Location> locationObject_;
    int basevalue_ = 0;
    bool isSelected = false;
    bool isHovered_ = false;

    // Resources are initially set to a constant NULL to avoid errors
    Interface::CommonResource localRes_ = NULLRES;
    Interface::CommonResource demandRes_ = NULLRES;

    int mapIndex_;

};

#endif // LOCATIONITEM_HH
