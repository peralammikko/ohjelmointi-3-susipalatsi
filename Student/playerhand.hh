#ifndef PLAYERHAND_HH
#define PLAYERHAND_HH

#include <QObject>
#include <QGraphicsScene>


#include "../Course/player.h"
// For now we use just simple card items
#include "mapitem.hh"


class PlayerHand : public mapItem
{
public:

    // This class is more or less deprecated and probably requires a removal.
    // stays here in case we get a nice idea how to use this as a hand display.
    PlayerHand(QGraphicsScene* scene, std::shared_ptr<const Interface::Player> player);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual const QString typeOf() override;

    void addMapItem(mapItem* mItem);

    void setActability(bool canAct);

    std::shared_ptr<const Interface::Player> getOwner();

    void rearrange() override;

private:
    QGraphicsScene* scene_;
    std::shared_ptr<const Interface::Player> player_;

    std::vector<mapItem*> items_;

    void arrangeAroundPoint(int startx, std::vector<mapItem *> mItems, int padding);
    
};

#endif // PLAYERHAND_HH
