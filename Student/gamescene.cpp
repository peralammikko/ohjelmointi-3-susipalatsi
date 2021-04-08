#include "gamescene.hh"
#include "gamewindow.hh"
#include "agentitem.hh"
#include "mapitem.hh"

GameScene::GameScene(QWidget *parent) : QGraphicsScene(parent)
{

}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QGraphicsItem* itemClicked = itemAt(event->scenePos(),QTransform());
        mapItem* locItem = qgraphicsitem_cast<mapItem*>(itemClicked);
        if (locItem) {
            locItem->mousePressEvent(event);
            selectedLocation = locItem;
            qDebug() << selectedLocation->getObject()->name();
        } else {
            selectedLocation = nullptr;
        }
    }
}
