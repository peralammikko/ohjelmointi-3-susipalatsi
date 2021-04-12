#ifndef CARDITEM_HH
#define CARDITEM_HH

#include "mapitem.hh"
#include "../Course/cardinterface.h"


class CardItem : public mapItem
{
public:
    // Pelialueella liikuteltava korttibjekti
    CardItem(std::weak_ptr<Interface::CardInterface> card, float scale=1);
    ~CardItem();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    // mouse entering and press events. These trigger when cards are in hand
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    // some cool hovering stuff
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;


    // This is called by gamescene when another mapitem is dragged over it
    void setHighLighted(bool state);


    const QString typeOf() override;


signals:
    void cardItemMoved(CardItem*);
    void cardItemDropped(CardItem* me);

private:
    int width_;
    int height_;
    std::pair<int, int> coords_;

    bool isPressed_;
    bool isHovered_;


    std::pair<int,int> coordsBeforeDragging_;

    std::weak_ptr<Interface::CardInterface> card_;


};

#endif // CARDITEM_HH
