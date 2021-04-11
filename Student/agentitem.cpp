#include "agentitem.hh"
#include "mapitem.hh"

#include <QDebug>

agentItem::agentItem(std::shared_ptr<Interface::AgentInterface> &obj) : agentObject_(nullptr), agentConnections_(0)
{
    agentObject_ = obj;
}

std::shared_ptr<Interface::AgentInterface> agentItem::getObject()
{
    return agentObject_;
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

}

void agentItem::setCoords(int x, int y)
{
    itemx = x;
    itemy = y;
}

const std::pair<int, int> agentItem::getCoords()
{
    std::pair<int, int> itemCoords (itemx, itemy);
    return itemCoords;
}

bool agentItem::isCommon() const
{
    return true;
}

unsigned short agentItem::connections() const
{
    return agentConnections_;
}

void agentItem::modifyConnections(short change)
{
    agentConnections_ += change;
}

std::weak_ptr<Interface::Location> agentItem::placement() const
{
    return locationAt_;
}

void agentItem::setConnections(unsigned short connections)
{
    agentConnections_ = 0 + connections;
}

void agentItem::setPlacement(std::weak_ptr<Interface::Location> placement)
{
    locationAt_ = placement;
}

QString agentItem::typeName() const
{
    return "Agentti";
}

QString agentItem::name() const
{
    return agentName_;
}

QString agentItem::title() const
{
    return "Agentti";
}

std::weak_ptr<Interface::Location> agentItem::location() const
{
    return locationAt_;
}

std::weak_ptr<Interface::Player> agentItem::owner() const
{
    return agentOwner_;
}

void agentItem::setOwner(std::weak_ptr<Interface::Player>)
{
    qDebug() << "set owner to ";
}


const QString agentItem::typeOf()
{
    return "agentitem";
}
