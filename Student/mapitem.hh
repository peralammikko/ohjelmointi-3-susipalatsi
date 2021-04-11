#ifndef MAPITEM_HH
#define MAPITEM_HH

#include <QGraphicsItem>
#include <QPainter>

#include <memory>
#include <map>
#include "../Course/simplemainwindow.hh"
#include "../Course/location.h"
#include "../Course/cardinterface.h"

class mapItem : public QGraphicsItem
{
public:
    // mapItem(const std::shared_ptr<Interface::Location> &obj);

    virtual QRectF boundingRect() const = 0;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) = 0;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) = 0;

    // Eikös nää oo turhia jos QGraphicsItemissä on jo setPos ja getPos?
    // virtual void setCoords(int x, int y) = 0;
    // virtual const std::pair<int, int> getCoords() = 0;

    // returns the type of MapItem as a string. Can be at least a Location or an ActionCard.
    virtual const QString typeOf() = 0;
};

#endif // MAPITEM_HH
