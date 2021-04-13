#include <QDebug>

#include "locationitem.hh"

LocationItem::LocationItem(const std::shared_ptr<Interface::Location> &location) : locationObject_(location), basevalue_(2)
{
    isSelected = false;
    isHovered_ = false;
    setAcceptHoverEvents(true);
}

void LocationItem::setCoords(int x, int y)
{
    this->itemx = x;
    this->itemy = y;
}

QRectF LocationItem::boundingRect() const
{
    return QRectF(itemx, itemy, 120,120);
}

void LocationItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Piirretään paikat ja niiden nimet.
    // Maalataan punaiseksi jos "valittu"

    QRectF rect = boundingRect();
    QPoint upperpos(itemx+35, itemy-5);
    QPoint lowerpos(itemx+30, itemy+160);
    QString placeName = this->getObject()->name();
    QPen pen(Qt::black, 2);
    painter->drawText(upperpos, placeName);
    painter->drawText(lowerpos, "Base value: " + QString::number(this->getBasevalue()));

    if (isHovered_) {
        pen.setColor(Qt::red);
    } else {
        pen.setColor(Qt::black);
    }
    painter->setPen(pen);
    painter->drawRect(rect);
}

const std::shared_ptr<Interface::Location> LocationItem::getObject()
{
    return locationObject_;
}

int LocationItem::getBasevalue()
{
    return basevalue_;
}

const std::pair<int, int> LocationItem::getCoords()
{
    std::pair<int, int> itemCoords (itemx, itemy);
    return itemCoords;
}

void LocationItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
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
}

void LocationItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    isHovered_ = true;
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void LocationItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    isHovered_ = false;
    update();
    QGraphicsItem::hoverLeaveEvent(event);
}

const QString LocationItem::typeOf()
{
    return "locationitem";
}
