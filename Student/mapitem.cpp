#include "mapitem.hh"
#include "location.h"
#include "gamewindow.hh"

#include <QDebug>

mapItem::mapItem(const std::shared_ptr<Interface::Location> &obj) : locationObject(obj)
{
}

void mapItem::setCoords(int x, int y)
{
    this->itemx = x;
    this->itemy = y;
}

QRectF mapItem::boundingRect() const
{
    return QRectF(itemx, itemy, 140,140);
}

void mapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();
    QPoint pos(itemx+35, itemy-5);
    QString placeName = this->getObject()->name();

    if (isSelected) {
        QPen pen(Qt::red, 2);
        painter->setPen(pen);
        painter->drawRect(rect);
        painter->drawText(pos, placeName);
    } else {
        QPen pen(Qt::black, 2);
        painter->setPen(pen);
        painter->drawRect(rect);
        painter->drawText(pos, placeName);
    }
}

const std::shared_ptr<Interface::Location> mapItem::getObject()
{
    return this->locationObject;
}

const std::pair<int, int> mapItem::getCoords()
{
    std::pair<int, int> itemCoords (itemx, itemy);
    return itemCoords;
}

/*
void mapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (isSelected == true) {
        isSelected = false;
        update();
        QGraphicsItem::mousePressEvent(event);
    } else {
        isSelected = true;
        update();
        QGraphicsItem::mousePressEvent(event);
    }
    clickedLocation = this;
}
*/


