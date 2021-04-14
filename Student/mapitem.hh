#ifndef MAPITEM_HH
#define MAPITEM_HH

#include <QGraphicsItem>
#include <QPainter>

#include <memory>
#include <map>
#include "../Course/simplemainwindow.hh"
#include "../Course/location.h"
#include "../Course/cardinterface.h"

class mapItem : public QObject, public QGraphicsItem
{
public:

    // QGraphicsItem overrides liekö edes tarpeellisia tässä?
    virtual QRectF boundingRect() const = 0;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) = 0;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) = 0;

    // returns the type of MapItem as a string. Can be at least a location, agent or an actionCard.
    // technically useless now that we know how to use dynamic_cast
    virtual const QString typeOf() = 0;

    // TODO: MouseHover and drag events go here (currently only CardItem has them, but would be nice that Agents would too!)
signals:
    virtual void mapItemMouseReleased(mapItem*);
    virtual void mapItemMouseDragged(mapItem*);

};

#endif // MAPITEM_HH
