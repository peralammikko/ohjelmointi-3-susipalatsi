#include "agentitem.hh"
#include "mapitem.hh"

#include <QDebug>

agentItem::agentItem(std::shared_ptr<agentCard> &obj) : mapItem(nullptr), agentCardObject(nullptr)
{
    agentCardObject = obj;
}

std::shared_ptr<agentCard> agentItem::getObject()
{
    return agentCardObject;
}

QRectF agentItem::boundingRect() const
{
    return QRectF(itemx, itemy, 50,100);
}

void agentItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();

    if (this->isSelected) {
        QPen pen(Qt::red, 2);
        painter->setPen(pen);
        painter->drawRect(rect);
    } else {
        QPen pen(Qt::black, 2);
        painter->setPen(pen);
        painter->drawRect(rect);
    }
}

void agentItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (this->isSelected == true) {
        this->isSelected = false;
        update();
        QGraphicsItem::mousePressEvent(event);
    } else {
        this->isSelected = true;
        update();
        QGraphicsItem::mousePressEvent(event);
    }
    clickedAgent = this;

}

