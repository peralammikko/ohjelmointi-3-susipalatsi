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
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void drawLocations(std::vector<std::shared_ptr<Interface::Location>> &locvec);
    void drawItem(mapItem* item);

private:
    mapItem* selectedLocation = nullptr;
    agentItem* selectedAgent = nullptr;
};

#endif // GAMESCENE_HH
