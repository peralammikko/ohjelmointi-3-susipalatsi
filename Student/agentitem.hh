#ifndef AGENTITEM_HH
#define AGENTITEM_HH

#include "mapitem.hh"
#include "agentinterface.h"

class agentItem : public mapItem, public Interface::AgentInterface
{
public:
    agentItem(QString name);
    bool isSelected = false;
    std::shared_ptr<Interface::AgentInterface> getAgent();
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    agentItem *clickedAgent = nullptr;

private:
    int itemx, itemy;
    std::shared_ptr<Interface::AgentInterface> agentObject;
    QString name_;
    int value_;
    int neuvottelukyky_;
};

#endif // AGENTITEM_HH
