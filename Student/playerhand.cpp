#include "playerhand.hh"


PlayerHand::PlayerHand(QGraphicsScene* scene, std::shared_ptr<Interface::Player> player) : scene_(scene), player_(player)
{

}

QRectF PlayerHand::boundingRect() const
{
    return QRectF(0,0,400,100);
}

void PlayerHand::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(Qt::red, 2);
    painter->setPen(pen);
    painter->drawRect(boundingRect());
}


void PlayerHand::addMapItem(mapItem* mItem)
{
    mItem->setParentItem(this);
    rearrange();
}

void PlayerHand::rearrange()
{
    float widthtotal = 0.0;
    int xStart = 0;
    float widthPerCard;

    float handPadding = 5;
    auto items = childItems();

    int count = items.size();
    if (count) {
        // Calculate total width of the hand
        for (int i = 0; i < count; ++i) {
            items.at(i)->show();
            widthtotal += items.at(i)->boundingRect().width();
        }
        // Gets the new coords for cards based on hand width
        widthPerCard = (widthtotal + handPadding*count) / count;
        xStart =  boundingRect().width()/2 - (widthtotal / 2);
        for (int i = 0; i < count; ++i) {
            auto mapitem = dynamic_cast<mapItem*>(items.at(i));
            if (mapitem)
            {
                int x = (xStart + widthPerCard*i);
                mapitem->setHome(QPointF(x, 0));
               // mapitem->setPos(x, boundingRect().height()/2);
                mapitem->goHome();
            }
           // items.at(i)->setParent(this);


        }
    }
}

