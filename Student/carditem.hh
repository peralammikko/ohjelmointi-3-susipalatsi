#ifndef CARDITEM_HH
#define CARDITEM_HH

#include "mapitem.hh"
#include "../Course/cardinterface.h"

namespace Interface {
    class CardInterface;
}

class CardItem : public mapItem
{
public:
    // Pelialueella liikuteltava korttibjekti
    CardItem(std::shared_ptr<Interface::CardInterface> card, QObject *parent);
    ~CardItem();

    std::shared_ptr<Interface::CardInterface> getCard();

    // Mandatory overrides
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    // This is called by gamescene when another mapitem is dragged over it
    void setHighLighted(bool state);

    // returns "card". This is useless probably.
    const QString typeOf() override;


    /*
signals:

    void something() const;
    void cardMoved(CardItem*);
    void cardReleased(CardItem*);

    virtual void mapItemMouseReleased(mapItem*) override;
    virtual void mapItemMouseDragged(mapItem*) override;
    */

protected:
    // mouse entering and press events
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    // some cool hovering stuff
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    // Interface from which this item takes all its data (name, owner etc.)
    std::shared_ptr<Interface::CardInterface> card_;

    int width_;
    int height_;
    std::pair<int, int> coords_;

    bool isPressed_;
    bool isHovered_;


    std::pair<float,float> coordsBeforeDragging_;



};

#endif // CARDITEM_HH
