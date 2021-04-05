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
    mapItem(const std::shared_ptr<Interface::Location> &obj);
    void setCoords(int x, int y);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    bool isSelected = false;
    const std::shared_ptr<Interface::Location> getObject();
    const std::pair<int, int> getCoords();
    mapItem *clickedLocation = nullptr;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int itemx, itemy;
    const std::shared_ptr<Interface::Location> locationObject;
};

#endif // MAPITEM_HH
