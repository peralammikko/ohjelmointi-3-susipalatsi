#ifndef AGENTITEM_HH
#define AGENTITEM_HH
#include <QDebug>

#include "mapitem.hh"
#include "agent.hh"
#include "../Course/agentinterface.h"

class agentItem : public mapItem
{
public:
    agentItem(std::shared_ptr<Interface::Agent> &agentinterface);
    ~agentItem();

    bool isSelected = false;
    std::shared_ptr<Interface::AgentInterface> getObject();

    // MapItem overridet
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    void testPrint() {qDebug() << "Tämä on agentti";}

    const QString typeOf() override;

protected:
    // mouse entering and press events
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    // some cool hovering stuff
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    // The object from which this card takes its data (name, owner etc)
    std::shared_ptr<Interface::Agent> agentObject_;

    // not sure if these are useful or not
    std::weak_ptr<Interface::Location> locationAt_;
    std::weak_ptr<Interface::Player> agentOwner_;
    QString agentName_;
    int value_;
    int negSkill_;
    int agentConnections_;

    // mouse event variables
    bool pressed_;

};

#endif // AGENTITEM_HH
