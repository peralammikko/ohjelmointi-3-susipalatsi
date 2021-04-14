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
    QRectF rect = boundingRect();
    // the text should always be withing boundingrect
    painter->drawText(5,10, agentObject_->name() );

    if (isSelected) {
        QPen pen(Qt::red, 2);
        painter->setPen(pen);
        painter->drawRect(rect);
    } else {
        QPen pen(Qt::black, 2);
        painter->setPen(pen);
        painter->drawRect(rect);
    }
}




const QString agentItem::typeOf()
{
    return "agentitem";
}


// mouse entering and press events

void agentItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pressed_ = true;
    update();
    QGraphicsItem::mousePressEvent(event);
}

void agentItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    pressed_ = false;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void agentItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseMoveEvent(event);
}

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
