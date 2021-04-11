#include "carditem.hh"
#include "QtDebug"
CardItem::CardItem(std::weak_ptr<Interface::CardInterface> card, float scale)
{
    setFlag(ItemIsMovable);
    isPressed_ = false;
    isHovered_ = false;

    // Required for mousehovering magics
    setAcceptHoverEvents(true);
}

CardItem::~CardItem()
{

}

QRectF CardItem::boundingRect() const
{
    return QRectF(0,0,100,200);
}


void CardItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec = boundingRect();
    QBrush brush(Qt::black);
    if (isPressed_) {
        brush.setColor(Qt::red);
    }

    if (isHovered_) {
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
    setPos(rect.topLeft());
}


const QString CardItem::typeOf()
{
    return "carditem";
}


void CardItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << "sekx ";
    isHovered_ = true;
    update();
    QGraphicsItem::hoverEnterEvent(event);
}


void CardItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << "batang";
    isHovered_ = false;
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

