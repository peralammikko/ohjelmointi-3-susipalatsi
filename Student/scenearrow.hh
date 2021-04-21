#ifndef SCENEARROW_HH
#define SCENEARROW_HH
#include <QGraphicsLineItem>
#include "mapitem.hh"


class SceneArrow : public QGraphicsLineItem
{
public:
    // When actions are selected, graphicsview spawns two arrows to point from where map item is going and where it came from
    // This has been mostly done with the help of official doc.qt.io tutorial qtwidgets-graphicsview-diagramscene-example.html#arrow-class-definition
    // Full link to the tutorial: https://doc.qt.io/qt-5/qtwidgets-graphicsview-diagramscene-example.html#arrow-class-definition
    SceneArrow(mapItem *startItem, mapItem *endItem, QGraphicsItem *parent = nullptr);
    void setColor(const QColor &color){color_=color;}

    void setStartItem(mapItem* startItem){startItem_=startItem;}
    void setEndItem(mapItem* endItem){endItem_=endItem;}
    void updatePosition();
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;
    virtual void advance(int phase) override;

private:
    mapItem *startItem_;
    mapItem *endItem_;
    QPolygonF arrowHead;
    QColor color_ = Qt::black;

};

#endif // SCENEARROW_HH
