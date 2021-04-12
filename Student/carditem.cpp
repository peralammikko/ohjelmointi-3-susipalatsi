#include "carditem.hh"
#include "QtDebug"

#include "gamescene.hh"

CardItem::CardItem(std::weak_ptr<Interface::CardInterface> card, float scale)
{
    setFlag(ItemIsMovable);
    isPressed_ = false;
    isHovered_ = false;
    std::pair<int,int> coordsBeforeDragging_;

    // set origo center for scaling
    QPoint o;
    float w = boundingRect().width()/2;
    float y = boundingRect().height()/2;
    o.setX(o.x() + w);
    o.setY(o.y() + y);
    setTransformOriginPoint(o);

    // Required for mousehovering magics
    setAcceptHoverEvents(true);
}

CardItem::~CardItem()
{

}

QRectF CardItem::boundingRect() const
{
    return QRectF(0,0,150,220);
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

void CardItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    isPressed_ = true;
    update();
    QGraphicsItem::mousePressEvent(event);
}

void CardItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (isPressed_)
    {
       // qDebug() << "hello i am moved" << mapToScene(mapFromScene(QCursor::pos()))<< "mouse" << mapFromScene(QCursor::pos());
        GameScene* gameScene = qobject_cast<GameScene*> (scene());
        if (gameScene != nullptr)
        {
            gameScene->onCardDragged(this);
        } else
        {
           qDebug() << "error! Card Item did not find parent scene while moving!";
        }
    }
    QGraphicsItem::mouseMoveEvent(event);
}


void CardItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    isPressed_ = false;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}


void CardItem::setHighLighted(bool state)
{
    isHovered_ = state;
}

const QString CardItem::typeOf()
{
    return "carditem";
}

int CardItem::type() const
{
    return Type;
}



void CardItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    isHovered_ = true;
    QPoint o;
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

