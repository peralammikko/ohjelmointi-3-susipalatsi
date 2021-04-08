#ifndef GAMESCENE_HH
#define GAMESCENE_HH

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QList>

#include "game.h"
#include "agentitem.hh"
#include "mapitem.hh"

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GameScene(QWidget* parent = nullptr);
    agentItem *agentClicked(agentItem* &age);
    mapItem *locationClicked(mapItem* &loc);
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // GAMESCENE_HH
