#ifndef MAPITEM_HH
#define MAPITEM_HH

#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include <memory>
#include <map>  // what is this for?

#include "../Course/location.h"
#include "../Course/cardinterface.h"



namespace Interface {
    class CardInterface;
}


class mapItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:

    // QGraphicsItem overridet liekö edes tarpeellisia tässä?
    virtual QRectF boundingRect() const = 0;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) = 0;

    // returns the type of MapItem as a string. Can be at least a location, agent or an actionCard.
    // technically useless now that we know how to use dynamic_cast
    virtual const QString typeOf() = 0;

    // mapitem is moved to homeCoordinatesOnScene_ in time milliseconds
     // TODO: animation
    virtual void goHome(int time=50);

protected:
    // in goHome the card is moved back to this point on its parent scene
    QPointF homeCoordinatesOnScene_ = QPointF(0,0);

    // Mouse moving event. In order for these to run, a child must have  Q_UNUSED(QWidget *parent) or similar in its constructor
    // If the item needs to be moved, it also reuires following line called somewhere
    //     setFlags(ItemIsMovable | ItemIsSelectable);

    // When item is clicked on a left mouse button and the item is not already pressed sets isMousePressed_ to true
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    // Emits mapItemMouseDragged(mapItem*) if moving while isMousePressed_ is true
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    // Emits mapItemMouseReleased(mapItem*) if a dragging leftmousebutton is released and isMousePressed
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    bool isMousePressed_ = 0;


signals:
    void mapItemMouseReleased(mapItem*);
    void mapItemMouseDragged(mapItem*);

};

#endif // MAPITEM_HH
