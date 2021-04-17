#ifndef AGENTITEM_HH
#define AGENTITEM_HH
#include <QDebug>
#include <QTimer>

#include "mapitem.hh"
#include "agent.hh"
#include "../Course/agentinterface.h"
#include "agentdialog.hh"

class agentItem : public mapItem
{
public:
    agentItem(std::shared_ptr<Interface::Agent> &agentinterface);
    ~agentItem();

    bool isSelected = false;
    std::shared_ptr<Interface::AgentInterface> getObject();
    std::shared_ptr<Interface::Agent> getAgentClass();

    // MapItem overridet
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    const QString typeOf() override;

    void spawnDialogue(std::shared_ptr<Interface::Agent> agent);

protected:
    // some cool hovering stuff if these are needed.
    // TODO: these do nothing now
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    // The object from which this card takes its data (name, owner etc)
    std::shared_ptr<Interface::Agent> agentObject_;

    // not sure if these are useful or not
    std::weak_ptr<Interface::Location> locationAt_;
    std::weak_ptr<Interface::Player> agentOwner_;

    // This should probably be moved to agent.hh
    int agentConnections_;

    // Empty dialog window to be called on mouse hover on/off
    AgentDialog* dialog_ = nullptr;

    // Timer used to spawn agent info dialog
    QTimer timer_;
    int delay_ = 1;

};

#endif // AGENTITEM_HH
