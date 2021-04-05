#include "agentitem.hh"
#include "mapitem.hh"

#include <QDebug>

agentItem::agentItem(QString name) : mapItem(nullptr), agentObject(nullptr), value_(0)
{
    name_ = name;
}

std::shared_ptr<Interface::AgentInterface> agentItem::getAgent()
{
    return agentObject;
}

QRectF agentItem::boundingRect() const
{
    return QRectF(itemx, itemy, 50,100);
}

void agentItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();

    if (this->isSelected) {
        QPen pen(Qt::red, 2);
        painter->setPen(pen);
        painter->drawRect(rect);
    } else {
        QPen pen(Qt::black, 2);
        painter->setPen(pen);
        painter->drawRect(rect);
    }
}

void agentItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (this->isSelected == true) {
        this->isSelected = false;
        update();
        QGraphicsItem::mousePressEvent(event);
    } else {
        this->isSelected = true;
        update();
        QGraphicsItem::mousePressEvent(event);
    }
    clickedAgent = this;

    // Agentin lähetys sijaintiin (WIP)
    /*
    if (clickedAgent->isSelected and mapItem::clickedLocation->isSelected) {
        qDebug() << "paikka ja agentti valittu";
        clickedLocation->getObject()->sendAgent(this->getAgent());
        auto setti = clickedLocation->getObject()->agents();
        for (auto i : setti) {
            qDebug() << i->name();
        }
    }
    */
}

