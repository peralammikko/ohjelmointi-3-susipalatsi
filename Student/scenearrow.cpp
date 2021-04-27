#include "scenearrow.hh"

#include <QDebug>

SceneArrow::SceneArrow(mapItem *startItem, mapItem *endItem, QGraphicsItem *parent)
    : startItem_(startItem), endItem_(endItem)
{
    setPen(QPen(color_, 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

void SceneArrow::updatePosition()
{
    if ( startItem_ and endItem_ ){
        auto startRect = startItem_->boundingRect();
        auto endRect = endItem_->boundingRect();
        QPointF sItemCenter = QPointF(startRect.width()/2, startRect.height()/2);
        QPointF eItemCenter = QPointF(endRect.width()/2, endRect.height()/2);
        setLine(QLineF(mapFromItem(startItem_, sItemCenter), mapFromItem(endItem_, eItemCenter)));
        show();
    } else {
        hide();
    }
}

void SceneArrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen p = pen();
    p.setColor(color_);
    painter->setPen(p);
    painter->setBrush(color_);
    painter->drawLine(line());
}

QRectF SceneArrow::boundingRect() const
{
    // Credits for this part: QT's official qtwidgets-graphicsview-diagramscene-example
    // I would love to change this part somehow so it would be really written by me, but honestly I have no idea.
    // I suppose I just could make the bounding rect REALLY big? Does not sound very efficient and I liked this tutorial anyway
    // Link to the tutorial:
    // https://doc.qt.io/qt-5/qtwidgets-graphicsview-diagramscene-example.html#arrow-class-definition
    qreal extra = (pen().width() + 20) / 2.0;

     return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                       line().p2().y() - line().p1().y()))
         .normalized()
             .adjusted(-extra, -extra, extra, extra);
}

void SceneArrow::advance(int phase)
{
    updatePosition();
}
