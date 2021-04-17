#ifndef AGENTITEM_HH
#define AGENTITEM_HH
#include <QDebug>

#include "mapitem.hh"
#include "agent.hh"
#include "../Course/agentinterface.h"

#include "sendagentaction.hh"

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

    const QString typeOf() override;

protected:
    // some cool hovering stuff if these are needed.
    // TODO: these do nothing now
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    virtual std::shared_ptr<Interface::ActionInterface> getDragReleaseAction() override;

signals:
    void actionSent(SendAgentAction action);
   // Interface::SendAgentAction(LocationItem *newLocItem, agentItem *aItem, LocationItem *oldLocItem=nullptr) asd;

private:
    // The object from which this card takes its data (name, owner etc)
    std::shared_ptr<Interface::Agent> agentObject_;

    // not sure if these are useful or not
    std::weak_ptr<Interface::Location> locationAt_;
    std::weak_ptr<Interface::Player> agentOwner_;

    // This should probably be moved to agent.hh
    int agentConnections_;

};

#endif // AGENTITEM_HH
