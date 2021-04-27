#ifndef PLAYERHAND_HH
#define PLAYERHAND_HH

#include <QObject>
#include <QGraphicsScene>
#include "../Course/player.h"
#include "mapitem.hh"
#include "carditem.hh"
#include "agentitem.hh"

/**
 * @file
 * @brief Graphic Card Item for actioncards
 */


class CardItem;

class PlayerHand : public mapItem
{
public:

    // This class is more or less deprecated and probably requires a removal.
    // stays here in case we get a nice idea how to use this as a hand display.
    PlayerHand(std::shared_ptr<const Interface::Player> player);
    ~PlayerHand();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual const QString typeOf() override;

    void addMapItem(mapItem* mItem);

    void setActability(bool canAct);

    std::shared_ptr<const Interface::Player> getOwner();

    std::vector<agentItem *> getAgentItems();
    std::vector<CardItem *> getCardItems();

    void removeActionCards();

    void rearrange() override;

private:
    std::shared_ptr<const Interface::Player> player_;

    QPixmap* handPixmap_;
    std::vector<mapItem*> items_;

    QColor playerColor_;

    void arrangeAroundPoint(int startx, std::vector<mapItem *> mItems, int padding);
    
};

#endif // PLAYERHAND_HH
