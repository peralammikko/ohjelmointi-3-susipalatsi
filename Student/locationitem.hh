#ifndef LOCATIONITEM_HH
#define LOCATIONITEM_HH

#include <QMainWindow>
#include <QObject>

#include "mapitem.hh"

class LocationItem : public mapItem
{
public:
    LocationItem(const std::shared_ptr<Interface::Location> &location);

    // MapItem overridet
    void setCoords(int x, int y) override;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    const std::pair<int, int> getCoords() override;


    bool isSelected = false;
    const std::shared_ptr<Interface::Location> getObject();

private:
    int itemx, itemy;
    const std::shared_ptr<Interface::Location> locationObject_;
};

#endif // LOCATIONITEM_HH
