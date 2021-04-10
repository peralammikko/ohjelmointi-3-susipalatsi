#ifndef MAPITEM_HH
#define MAPITEM_HH

#include <QGraphicsItem>
#include <QPainter>

#include <memory>
#include <map>
#include "../Course/simplemainwindow.hh"
#include "../Course/location.h"

class mapItem : public QGraphicsItem
{
public:
    // mapItem(const std::shared_ptr<Interface::Location> &obj);
    virtual void setCoords(int x, int y) = 0;
    virtual QRectF boundingRect() const = 0;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) = 0;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) = 0;
    virtual const std::pair<int, int> getCoords() = 0;
    const std::shared_ptr<Interface::Location> getObject();

    bool isSelected = false;

private:
    int itemx, itemy;
};

#endif // MAPITEM_HH
