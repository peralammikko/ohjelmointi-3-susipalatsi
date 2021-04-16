#include "agentitem.hh"
#include "agent.hh"
#include "mapitem.hh"

#include <QDebug>



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
/*
 *These have been moved to mapItem and are waiting for safe removal
 *
void agentItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Make sure it is a left button event and the item is not pressed
    if (event->button() == Qt::LeftButton and not pressed_)
    {
        pressed_ = true;
        homeCoordinatesOnScene_ = pos();
    }
    update();
    QGraphicsItem::mousePressEvent(event);
}

void agentItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (pressed_)
    {
        emit mapItemMouseDragged(this);
        update();
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void agentItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // Make sure it is a left button event and the item is pressed
    if (event->button() == Qt::LeftButton and pressed_)
    {
        pressed_ = false;
        emit mapItemMouseReleased(this);
    }
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}*/

// some cool hovering stuff
void agentItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    isSelected = true;
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void agentItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    isSelected = false;
    update();
    QGraphicsItem::hoverLeaveEvent(event);
}
