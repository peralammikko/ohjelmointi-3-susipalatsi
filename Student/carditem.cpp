#include "carditem.hh"
#include "QtDebug"

#include "gamescene.hh"

CardItem::CardItem(std::shared_ptr<Interface::CardInterface> card, QObject *parent)
{
    Q_UNUSED(parent);

    card_ = card;
    setFlags(ItemIsMovable | ItemIsSelectable);
    isPressed_ = false;
    isHovered_ = false;
    // This will be useful when we want card to be snapped back after dragging
    homeCoordinatesOnScene_ = QPointF(x(), y());

    // set origo center for scaling
    QPoint o;
    float w = boundingRect().width()/2;
    float y = boundingRect().height()/2;
    o.setX(o.x() + w);
    o.setY(o.y() + y);
    //setTransformOriginPoint(o);

    // Required for mousehovering magics
    setAcceptHoverEvents(true);
}

CardItem::~CardItem()
{

}

std::shared_ptr<Interface::CardInterface> CardItem::getCard()
{
    return card_;
}


QRectF CardItem::boundingRect() const
{
    return QRectF(0,0,80,120);
}

void CardItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec = boundingRect();
    QBrush brush(Qt::gray);
    if (isPressed_) {
        brush.setColor(Qt::green);
    }

    if (isHovered_ and not isPressed_) {
        brush.setColor(Qt::yellow);
    }

    painter->fillRect(rec, brush);
    painter->drawRect(rec);
}
/*
void CardItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Make sure it is a left button event and the card is not pressed already
    if (event->button() == Qt::LeftButton and not isPressed_)
    {
        // Set home coordinates where the mapitem will return to
        isPressed_ = true;
        homeCoordinatesOnScene_ = pos();
    }
    update();
    QGraphicsItem::mousePressEvent(event);
}

void CardItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{    
    // TODO: Cards for whatever reason always snap back to either hand or center of graphics view when released and picked up again.

    if (isPressed_)
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

void CardItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // TODO: Cards for whatever reason always snap back to either hand or center of graphics view when released and picked up again.
    if (isPressed_)
    {
        emit mapItemMouseReleased(this);
        isPressed_ = false;
    }
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
*/

void CardItem::setHighLighted(bool state)
{
    isHovered_ = state;
}

const QString CardItem::typeOf()
{
    return "carditem";
}

void CardItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    isHovered_ = true;
    setScale(1.2);
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void CardItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    isHovered_ = false;
    setScale(1);
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

