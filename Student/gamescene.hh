#ifndef GAMESCENE_HH
#define GAMESCENE_HH

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QGraphicsScene>

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GameScene();
};

#endif // GAMESCENE_HH
