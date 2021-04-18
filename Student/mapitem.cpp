#include "mapitem.hh"
#include "location.h"
#include "gamewindow.hh"

#include <QDebug>

void mapItem::goHome(int time)
{
    // TODO: implement animation movement function
    homingTimer_ = new QTimer(this);
    homingTimer_->setSingleShot(true);
    homing_ = true;
    homingTimer_->start(time);
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

void mapItem::advance(int phase)
{
    if (homing_ and homingTimer_){
        if (homingTimer_->remainingTime() <= 0)
        {
            homing_ = false;
            setPos(homeCoordinatesOnScene_);
        }
        QPointF toscene = mapFromScene(homeCoordinatesOnScene_);

        QPointF distanceLeft = homeCoordinatesOnScene_-pos();

        float xvelocity = distanceLeft.x() / (homingTimer_->remainingTime()*0.1);
        float yvelocity = distanceLeft.y() / (homingTimer_->remainingTime()*0.1);

        qDebug() << xvelocity << homingTimer_->remainingTime() << distanceLeft.x();
        setPos(x() + xvelocity, y() + yvelocity);
    }
}

void mapItem::setHome(QPointF newhome)
{
    homeCoordinatesOnScene_ = newhome;
}


