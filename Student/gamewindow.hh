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
#include "playerhand.hh"

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
    const std::vector<std::shared_ptr<Interface::Location>> getLocations();
    void setSize(int width, int height);
    void drawLocations();
    void drawItem(mapItem* item);
    void showHand();

    void enablePlayerHand(std::shared_ptr<Interface::Player> player);

private:
    Ui::GameWindow *gameui;
    GameScene *mapScene;
    std::shared_ptr<Interface::Game> gameboard = nullptr;
    std::shared_ptr<Interface::Runner> courseRunner = nullptr;

    std::map<std::shared_ptr<Interface::Player>, std::shared_ptr<PlayerHand>> hands_;

    const std::vector<QString> paikat_ = {"Marketti", "Kirkko", "Taverna", "Kauppiaiden kilta", "Menomesta", "Salapaikka"};

    int current_round;

};

#endif // GAMEWINDOW_HH
