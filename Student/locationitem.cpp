#include "locationitem.hh"

LocationItem::LocationItem(const std::shared_ptr<Interface::Location> &location) : locationObject_(location)
{

}

void LocationItem::setCoords(int x, int y)
{
    this->itemx = x;
    this->itemy = y;
}

QRectF LocationItem::boundingRect() const
{
    return QRectF(itemx, itemy, 140,140);
}

void LocationItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Piirretään paikat ja niiden nime.
    // Maalataan punaiseksi jos "valittu"

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

const std::shared_ptr<Interface::Location> LocationItem::getObject()
{
    return this->locationObject_;
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

const QString LocationItem::typeOf()
{
    return "locationitem";
}
