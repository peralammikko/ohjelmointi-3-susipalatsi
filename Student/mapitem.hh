#ifndef MAPITEM_HH
#define MAPITEM_HH

#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include <memory>
#include <QTimer>

#include "agentactioninterface.hh"
#include "../Course/cardinterface.h"

namespace Interface {
    class CardInterface;
    class AgentActionInterface;
}

const std::vector<QColor> PlayerColors = { QColor(209, 60, 54), QColor(66, 135, 245),
                                           QColor(135, 57, 230), QColor(35, 166, 50),
                                           QColor(208, 217, 128), QColor(0, 255, 200),
                                           QColor(255, 140, 180), QColor(130, 81, 7) };


class mapItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    // returns the type of MapItem as a string. Can be at least a location, agent or an actionCard.
    // technically useless now that we know how to use dynamic_cast
    virtual const QString typeOf() = 0;

    // mapitem is moved to homeCoordinatesOnScene_ in time milliseconds
     // BUG: item should not be grabbable while this is in effect
    virtual void goHome(int time=350);

    // sets home to somewhere else
    virtual void setHome(QPointF newhome= QPoint(0,0));

    virtual void setWaitingForAction(bool state);

    virtual bool isWaitingForAction();

    // Re-arrange all child items in its own layout
    virtual void rearrange() = 0;

protected:
    // Coordinates in which the item will home to. It is in parent item's coordinate system, like pos() is.
    QPointF homeCoords_ = QPointF(0,0);

    // Mouse moving event. In order for these to run, a child must have  Q_UNUSED(QWidget *parent) or similar in its constructor
    // If the item needs to be moved, it also reuires following line called somewhere
    //     setFlags(ItemIsMovable | ItemIsSelectable);

    // When item is clicked on a left mouse button and the item is not already pressed sets isMousePressed_ to true
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    // Emits mapItemMouseDragged(mapItem*) if moving while isMousePressed_ is true
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    virtual std::shared_ptr<Interface::ActionInterface> getDragReleaseAction();

    // Emits mapItemMouseReleased(mapItem*) if a dragging leftmousebutton is released and isMousePressed
    // If the item is dragged, triest to send actionDeclared signal
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    virtual void advance(int phase) override;

    bool isMousePressed_ = 0;

    bool waitingForActionCard_ = false;

    QTimer* homingTimer_;
    bool homing_ = false;

signals:
    virtual void actionDeclared(std::shared_ptr<Interface::ActionInterface>, mapItem*, bool resetting);

    void mapItemMouseReleased(mapItem*);
    void mapItemMouseDragged(mapItem*);


};

#endif // MAPITEM_HH
