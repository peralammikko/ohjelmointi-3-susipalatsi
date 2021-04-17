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

    // setting up center sprite
    centerimage_ = new QPixmap();
    if (not  centerimage_->load(":default.jpg")) {
       qDebug() << "Image failed to load";
    } else {
        centerimage_ = new QPixmap(":default.jpg");
    }

    // set origo center for scaling
    QPoint o;
    float w = boundingRect().width()/2;
    float y = boundingRect().height()/2;
  //  o.setX(o.x() + w);
   // o.setY(o.y() + y);
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

    painter->drawText(5,18, card_->name());

    //qDebug() << centerimage_->height() << centerimage_->width();

    painter->drawPixmap(0, 20, boundingRect().width(), boundingRect().height()/5 *2.5,  *centerimage_);
}

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

