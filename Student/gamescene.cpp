#include "gamescene.hh"
#include "gamewindow.hh"
#include "agentitem.hh"
#include "mapitem.hh"

GameScene::GameScene(QWidget *parent) : QGraphicsScene(parent)
{

}

agentItem* GameScene::agentClicked(agentItem *&age)
{
    // qDebug() << age->getObject()->name();
    return age;
}

mapItem *GameScene::locationClicked(mapItem *&loc)
{
    qDebug() << loc->getObject()->name();
    return loc;
}


void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QGraphicsItem* itemClicked = itemAt(event->scenePos(),QTransform());
        mapItem* mapi = qgraphicsitem_cast<mapItem*>(itemClicked);
        if (mapi) {
            qDebug() << mapi->getObject()->name();
            mapi->update();
        } else {
            qDebug() << "nope";
        }
    }
}
