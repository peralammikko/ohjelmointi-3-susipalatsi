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
    if (isMousePressed_ )
    {
        GameScene* gameScene = qobject_cast<GameScene*> (scene());
        if (gameScene != nullptr)
        {
            emit mapItemMouseDragged(this);
        } else
        {
            // Something here if scene was not found
        }
    }
    update();
    QGraphicsItem::mouseMoveEvent(event);
}

void mapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
     // Make sure it is a left button event and the card is pressed already
    if (event->button() == Qt::LeftButton and isMousePressed_ )
    {
        emit mapItemMouseReleased(this);
        isMousePressed_  = false;
    }
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void mapItem::setHome(QPointF newhome)
{
    homeCoordinatesOnScene_ = newhome;
}
