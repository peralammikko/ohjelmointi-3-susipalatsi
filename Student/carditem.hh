#ifndef CARDITEM_HH
#define CARDITEM_HH

#include "mapitem.hh"
#include "../Course/cardinterface.h"
#include "cardpaymentaction.hh"


/**
 * @file
 * @brief visual representation of an action card
 */
namespace Interface {
    class CardInterface;
}

class CardItem : public mapItem
{
public:
    /**
     * @brief CardItem a card which can be moved around on the game screen. Mainly used to pay actions.
     * @param card the card interface with all of the relevant information about the card
     * @param parent QObject parent
     */
    CardItem(std::shared_ptr<Interface::CardInterface> card, QObject *parent);

    /**
     * @brief ~CardItem deconstructor
     */
    ~CardItem();

    /**
     * @brief getCard
     * @return Card interface with card information
     */
    std::shared_ptr<Interface::CardInterface> getCard();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    /**
     * @brief typeOf returns "carditem"
     * @return "carditem"
     */
    const QString typeOf() override;
    void rearrange() override{};

protected:
    /**
     * @brief hoverEnterEvent makes the card slightly larger and different coloured by making isHovered_ true which is checked in painter.
     * @param event
     */
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    /**
     * @brief hoverLeaveEvent sets isHovered to false
     * @param event
     */
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    /**
     * @brief card_ Interface from which this item takes all its data (name, owner etc.)
     */
    std::shared_ptr<Interface::CardInterface> card_;

    /**
     * @brief getDragReleaseAction gives cardpayment action only if it is dropped on an agent which is waiting for an action
     * @return
     */
    virtual std::shared_ptr<Interface::ActionInterface> getDragReleaseAction() override;

    bool isHovered_;
    QPixmap *centerimage_;
};

#endif // CARDITEM_HH
