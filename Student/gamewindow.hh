#ifndef GAMEWINDOW_HH
#define GAMEWINDOW_HH

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include "gamescene.hh"
#include "game.h"
#include "mapitem.hh"

#include "../Course/game.h"
#include "../Course/runner.h"

namespace Ui {
class GameWindow;
}

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();
    void setSize(int width, int height);
    void drawLocations();
    // void drawAgents(mapItem* &drawLocation);

    void drawPlayerHand();


private:
    Ui::GameWindow *gameui;
    QGraphicsScene *mapScene;
    std::shared_ptr<Interface::Game> courseGameScene = nullptr;
    std::shared_ptr<Interface::Runner> courseRunner = nullptr;

    const std::vector<QString> paikat_ = {"Marketti", "Kirkko", "Taverna", "Kauppiaiden kilta", "Menomesta", "Salapaikka"};

    int current_round;

};

#endif // GAMEWINDOW_HH
