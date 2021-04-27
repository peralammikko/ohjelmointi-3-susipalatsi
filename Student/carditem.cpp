#include "carditem.hh"
#include "QtDebug"

#include "gamescene.hh"

CardItem::CardItem(std::shared_ptr<Interface::ActionCard> card, QObject *parent, QString spritePath ="")
{
    Q_UNUSED(parent);

    card_ = card;
    setFlags(ItemIsMovable | ItemIsSelectable);
    isHovered_ = false;
    // This will be useful when we want card to be snapped back after dragging
    homeCoords_ = QPointF(x(), y());

    if (spritePath == ""){
        spritePath = ":/img/governors/img/governors/1.png";
    }

    centerimage_ = new QPixmap(spritePath);

    // Required for mousehovering magics
    setAcceptHoverEvents(true);

}

CardItem::~CardItem()
{
    delete centerimage_;
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


    if (isHovered_) {
        brush.setColor(Qt::yellow);
    }

    painter->fillRect(rec, brush);
    painter->drawRect(rec);
    painter->drawText(5,18, card_->name());

    painter->drawPixmap(0, 20, boundingRect().width(), boundingRect().height()/5 *2.5,  *centerimage_);
   // painter->drawText(5,boundingRect().height()/5 *2.5, card_->description());
}

const QString CardItem::typeOf()
{
    return "actioncard";
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

std::shared_ptr<Interface::ActionInterface> CardItem::getDragReleaseAction()
{
    std::shared_ptr<Interface::ActionInterface> action;
    auto collisions = collidingItems();
    for (int i = 0; i < collisions.size(); ++i)
    {
        agentItem* aItem = dynamic_cast<agentItem*>(collisions.at(i));
        if (aItem and aItem->isWaitingForAction())
        {
            qDebug() << "Carditem succesfully dragged on a waiting agent";
            action = std::make_shared<CardPaymentAction>(aItem);
        }
    }
    return action;
}

