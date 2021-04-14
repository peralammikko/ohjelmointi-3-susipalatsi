#include "mapitem.hh"
#include "location.h"
#include "gamewindow.hh"

#include <QDebug>

void mapItem::goHome(int time)
{
    // TODO: implement animation movement function
    setPos(homeCoordinatesOnScene_);
}


void mapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Make sure it is a left button event and the card is not pressed already
    if (event->button() == Qt::LeftButton and not isMousePressed_ )
    {
        // Set home coordinates where the mapitem will return to
        isMousePressed_  = true;
        homeCoordinatesOnScene_ = pos();
    }
    update();
    QGraphicsItem::mousePressEvent(event);
}

void mapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // TODO: Cards for whatever reason always snap back to either hand or center of graphics view when released and picked up again.

    if (isMousePressed_ )
    {
        GameScene* gameScene = qobject_cast<GameScene*> (scene());
        if (gameScene != nullptr)
        {
            emit mapItemMouseDragged(this);
        } else
        {
           qDebug() << "error! Card Item did not find parent scene while moving!";
        }
    }
    update();
    QGraphicsItem::mouseMoveEvent(event);
}

void mapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // TODO: Cards for whatever reason always snap back to either hand or center of graphics view when released and picked up again.
    if (isMousePressed_ )
    {
        emit mapItemMouseReleased(this);
        isMousePressed_  = false;
    }
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
