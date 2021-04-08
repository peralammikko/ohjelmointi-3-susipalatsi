#ifndef AGENTITEM_HH
#define AGENTITEM_HH

#include "mapitem.hh"
#include "agentinterface.h"
#include "agentcard.hh"

class agentItem : public mapItem
{
public:
    explicit agentItem(std::shared_ptr<agentCard> &obj);
    bool isSelected = false;
    std::shared_ptr<agentCard> getObject();
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    agentItem *clickedAgent = nullptr;

private:
    int itemx, itemy;
    std::shared_ptr<agentCard> agentCardObject;
};

#endif // AGENTITEM_HH
