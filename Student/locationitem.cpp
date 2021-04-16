#include <QDebug>

#include "locationitem.hh"

LocationItem::LocationItem(const std::shared_ptr<Interface::Location> location) : locationObject_(location), basevalue_(2), isSelected(false), isHovered_(false)
{
    setAcceptHoverEvents(true);


}

QRectF LocationItem::boundingRect() const
{
    return QRectF(0, 0, 120,120);
}

void LocationItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Piirretään paikat ja niiden nimet.
    // Maalataan punaiseksi jos "valittu"

    QRectF rect = boundingRect();
    painter->fillRect(rect, Qt::gray);

    QPoint upperpos(boundingRect().x()+5, 10);
    QPoint lowerpos(0, boundingRect().height()-10);
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
  ;
}

const std::shared_ptr<Interface::Location> LocationItem::getObject()
{
    return locationObject_;
}

int LocationItem::getBasevalue()
{
    return basevalue_;
}

void LocationItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit locationItemPressed(this);
    update();
    QGraphicsItem::mousePressEvent(event);
}

void LocationItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    isHovered_ = true;
    if (childItems().size())
    {
        setRotation(rotation()+45);
    }
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void LocationItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setRotation(0);
    isHovered_ = false;
    update();
    QGraphicsItem::hoverLeaveEvent(event);
}

const QString LocationItem::typeOf()
{
    return "locationitem";
}
