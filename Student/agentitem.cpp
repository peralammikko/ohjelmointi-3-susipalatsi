#include "agentitem.hh"
#include "agent.hh"
#include "mapitem.hh"
#include "locationitem.hh"

#include <QDebug>

class LocationItem;

agentItem::agentItem(std::shared_ptr<Interface::Agent> &agentInterface) : agentConnections_(0)
{
    agentObject_ = agentInterface;
    setFlags(ItemIsMovable | ItemIsSelectable);

    setAcceptHoverEvents(true);
}

agentItem::~agentItem()
{

}

std::shared_ptr<Interface::AgentInterface> agentItem::getObject()
{
    return agentObject_;
}

std::shared_ptr<Interface::Agent> agentItem::getAgentClass()
{
    return agentObject_;
}

QRectF agentItem::boundingRect() const
{
    return QRectF(0, 0, 100, 100);
}

void agentItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen;
    QRectF rect = boundingRect();
    // the text should always be withing boundingrect
    painter->drawText(5,10, agentObject_->name() );

    if (isSelected) {
        QPen pen(Qt::red, 2);
    } else {
        QPen pen(Qt::black, 2);
    }
    painter->setPen(pen);
    painter->drawRect(rect);
}

const QString agentItem::typeOf()
{
    return "agentitem";
}

void agentItem::spawnDialogue(std::shared_ptr<Interface::Agent> agent)
{
    dialog_ = new AgentDialog(agent);
    dialog_->show();
}

void agentItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    isSelected = true;
    update();
    std::shared_ptr<Interface::Agent> agentPtr = this->getAgentClass();
    dialog_ = new AgentDialog(agentPtr);
    dialog_->show();
    QGraphicsItem::hoverEnterEvent(event);
}

void agentItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    isSelected = false;
    update();
    dialog_->close();
    dialog_ = nullptr;
    QGraphicsItem::hoverLeaveEvent(event);
}

std::shared_ptr<Interface::ActionInterface> agentItem::getDragReleaseAction()
{
    std::shared_ptr<Interface::ActionInterface> action;
    auto collisions = collidingItems();
    for (int i = 0; i < collisions.size(); ++i)
    {
        LocationItem* lItem = dynamic_cast<LocationItem*>(collisions.at(i));
        if (lItem)
        {
            action = std::make_shared<SendAgentAction>(lItem, this);
        }
    }
    return action;
}
