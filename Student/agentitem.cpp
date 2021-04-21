#include "agentitem.hh"
#include "agent.hh"
#include "mapitem.hh"
#include "locationitem.hh"

#include "agentactioninterface.hh"
#include "sendagentaction.hh"
#include "withdrawagentaction.hh"

#include <QDebug>

class LocationItem;
class AgentActionInterface;

agentItem::agentItem(std::shared_ptr<Interface::Agent> &agentInterface) : agentConnections_(0)
{
    agentObject_ = agentInterface;
    setFlags(ItemIsMovable | ItemIsSelectable);
    homing_ = false;
    timer_ = new QTimer(this);
    setAcceptHoverEvents(true);
}

agentItem::~agentItem()
{

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
    painter->drawText(5, 10, agentObject_->name());

    if (isSelected) {
        QPen pen(Qt::red, 2);
    } else {
        QPen pen(Qt::black, 2);
    }
    painter->setPen(pen);
    painter->drawEllipse(rect);

}

const QString agentItem::typeOf()
{
    return "agentitem";
}

void agentItem::spawnDialogue()
{
    // If cursor is no longer on agent, dialog is not shown
    if (!dialog_)
    {
        return;
    }
    if (isSelected) {
        dialog_->show();
    } else {
        dialog_ = nullptr;
    }
}

void agentItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (dialog_) {
        dialog_->close();
    }
    isSelected = true;
    update();
    if (not homing_){
        // Find out which agent was pointed at and get it's Agent class object
        std::shared_ptr<Interface::Agent> agentPtr = this->getAgentClass();

        if (not dialog_)
        {
            // Creating a new dialog window for said agent
            dialog_ = new AgentDialog(agentPtr);
            dialog_->move(event->pos().x(), event->pos().y());

            // Delay the popup for 1 second
            QTimer::singleShot(dialogDelay_, this, &agentItem::spawnDialogue);
            QGraphicsItem::hoverLeaveEvent(event);
        }

    }
}

void agentItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    isSelected = false;
    update();
    if (dialog_)
    {
        dialog_->close();
        delete dialog_;
        dialog_ = nullptr;
    }
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

        } else {
            PlayerHand* pHand = dynamic_cast<PlayerHand*>(collisions.at(i));
            if (pHand)
            {
                action =std::make_shared<WithdrawAgentAction>(pHand, this);
            }
        }
    }
    return action;
}
