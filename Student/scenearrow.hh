#ifndef SCENEARROW_HH
#define SCENEARROW_HH
#include <QGraphicsLineItem>
#include "mapitem.hh"


class SceneArrow : public QGraphicsLineItem
{
public:

    // DISCLAIMER:
    // This has been mostly done with the help of official doc.qt.io tutorial qtwidgets-graphicsview-diagramscene-example.html#arrow-class-definition
    // Full link to the tutorial: https://doc.qt.io/qt-5/qtwidgets-graphicsview-diagramscene-example.html#arrow-class-definition
    /**
     * @brief SceneArrow When actions are selected, graphicsview spawns two arrows to point from where map item is going and where it came from
     * @param startItem control point parent item for the arrow
     * @param endItem control point parent item for the arrow
     * @param parent
     */
    SceneArrow(mapItem *startItem, mapItem *endItem, QGraphicsItem *parent = nullptr);



    /**
     * @brief setStartItem the item which the arrow is "rooted to". It's center serves as a control point
     * @param startItem
     */
    void setStartItem(mapItem* startItem){startItem_=startItem;}

    /**
     * @brief setStartItem the other control point item
     * @param startItem
     */
    void setEndItem(mapItem* endItem){endItem_=endItem;}

    void setColor(const QColor &color){color_=color;}
    void updatePosition();
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;
    virtual void advance(int phase) override;

private:
    mapItem *startItem_;
    mapItem *endItem_;
    QColor color_ = Qt::yellow;

};

#endif // SCENEARROW_HH
