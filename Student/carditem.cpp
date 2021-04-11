#include "carditem.hh"
#include "QtDebug"
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

void CardItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    isPressed_ = false;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

QSizeF CardItem::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_UNUSED(which);
    Q_UNUSED(constraint);
    return boundingRect().size();
}

void CardItem::setGeometry(const QRectF &rect)
{
    setPos(rect.center());
}


const QString CardItem::typeOf()
{
    return "carditem";
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

