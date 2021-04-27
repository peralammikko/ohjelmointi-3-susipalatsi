#include "mapitem.hh"
#include "location.h"
#include "gamewindow.hh"

void mapItem::goHome(int time)
{
    if (homingTimer_ != nullptr){
        delete homingTimer_;
    }
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
        homing_ = false;
        //homeCoordinatesOnScene_ = pos();
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
    update();
    QGraphicsItem::mouseReleaseEvent(event);
     // Make sure it is a left button event and the card is pressed already
    if (event->button() == Qt::LeftButton and isMousePressed_ )
    {
        isMousePressed_  = false;

        // TODO: maybe move perform checking somewhere else
        if (isWaitingForAction())
        {
            emit actionDeclared(std::shared_ptr<Interface::ActionInterface>(), this, true);
            return;
        }
        std::shared_ptr<Interface::ActionInterface>  action = getDragReleaseAction();
        if (action and action->canPerform())
        {
            emit actionDeclared(getDragReleaseAction(), this, false);
        } else {
            this->goHome();
        }
    }
}

void mapItem::setWaitingForAction(bool state)
{
    waitingForActionCard_ = state;
}

bool mapItem::isWaitingForAction()
{
    return waitingForActionCard_;
}

void mapItem::advance(int phase)
{
    if (homing_){
        if (!homingTimer_ or homingTimer_->remainingTime() <= 0)
        {
            homing_ = false;
            setPos(homeCoords_);

        } else {
            QPointF distanceLeft = homeCoords_-pos();

            float xvelocity = distanceLeft.x() / (homingTimer_->remainingTime()*0.05);
            float yvelocity = distanceLeft.y() / (homingTimer_->remainingTime()*0.05);

            setPos(pos() + QPointF(xvelocity, yvelocity));
        }
    }
}

void mapItem::setHome(QPointF newhome)
{
    homeCoords_ = newhome;
}


