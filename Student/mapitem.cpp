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

std::shared_ptr<Interface::ActionInterface> mapItem::getDragReleaseAction()
{
    return std::shared_ptr<Interface::ActionInterface>();
}

void mapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
     // Make sure it is a left button event and the card is pressed already
    if (event->button() == Qt::LeftButton and isMousePressed_ )
    {
        //emit mapItemMouseReleased(this);
        qDebug() << this->collidingItems();
        isMousePressed_  = false;

        // TODO: maybe move perform checking somewhere else
        std::shared_ptr<Interface::ActionInterface> action = getDragReleaseAction();
        if (action and action->canPerform()){
            emit actionDeclared(getDragReleaseAction());
        } else {
            this->goHome();
        }
    }
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void mapItem::setHome(QPointF newhome)
{
    homeCoordinatesOnScene_ = newhome;
}
