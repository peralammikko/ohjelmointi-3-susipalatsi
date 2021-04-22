#include "playerhand.hh"
#include "carditem.hh"
#include "agentitem.hh"

PlayerHand::PlayerHand(QGraphicsScene* scene, std::shared_ptr<const Interface::Player> player) : scene_(scene), player_(player)
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

const QString PlayerHand::typeOf()
{
    return "playerhand";
}


void PlayerHand::addMapItem(mapItem* mItem)
{
    mItem->setParentItem(this);
    rearrange();
}

void PlayerHand::setActability(bool canAct)
{

}

std::shared_ptr<const Interface::Player> PlayerHand::getOwner()
{
    return player_;
}

void PlayerHand::removeActionCards()
{
    QList<QGraphicsItem *> const items = childItems();
    int count = items.size();
    if (count) {
        std::vector<mapItem*> cItems;
        // Separate cards from agents and get their total width
        for (int i = 0; i < count; ++i) {
        auto mItem = dynamic_cast<CardItem*>(items.at(i));
            if (mItem){
                cItems.push_back(mItem);
                auto cInterface = mItem->getCard();
                cInterface->owner().lock()->playCard(cInterface);
            }
        }
        for (unsigned int i = 0; i < cItems.size(); ++i){
            delete cItems.at(i);
        }
    }
}

void PlayerHand::rearrange()
{
    update();
    int xStartForCards = 0;

    float handPadding = 5;
    float paddingBetweenAgentsAndCards = 50;

    std::vector<agentItem*> aItems;
    float agentWidthTotal = 0.0;
    std::vector<CardItem*> cItems;
    float cardWidthTotal = 0.0;

    QList<QGraphicsItem *> const items = childItems();
    int count = items.size();
    if (count) {

        std::vector<mapItem*> aItems;
        std::vector<mapItem*> cItems;

        // Separate cards from agents and get their total width
        for (int i = 0; i < count; ++i) {
            //items.at(i)->show();
            auto mItem = dynamic_cast<agentItem*>(items.at(i));
            if (mItem){
                aItems.push_back(mItem);
                agentWidthTotal += mItem->boundingRect().width() + handPadding;
            } else {
                auto mItem = dynamic_cast<CardItem*>(items.at(i));
                if (mItem){
                    cItems.push_back(mItem);
                    cardWidthTotal += mItem->boundingRect().width() + handPadding;
                }
            }
        }
        if (!cItems.size() or !aItems.size())
        {
            paddingBetweenAgentsAndCards = 0;
        }

        int totalWidth  = cardWidthTotal + agentWidthTotal + paddingBetweenAgentsAndCards;
        xStartForCards = boundingRect().width()/2 -totalWidth/2;
        int xStartForAgents = xStartForCards + cardWidthTotal + paddingBetweenAgentsAndCards;

        arrangeAroundPoint(xStartForCards, cItems, handPadding);
        arrangeAroundPoint(xStartForAgents, aItems, handPadding);
    }
}

void PlayerHand::arrangeAroundPoint(int startx, std::vector<mapItem*> mItems, int padding)
{
    int x;
    int passedWidth = 0;
    for (unsigned i = 0; i < mItems.size(); ++i)
    {
        auto item = mItems.at(i);
        x = startx + padding*i + passedWidth;
        item->setHome(QPointF(x,0));
        item->goHome();
        passedWidth += item->boundingRect().width();
    }
}
