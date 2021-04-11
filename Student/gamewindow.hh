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
#include "locationitem.hh"

#include "../Course/game.h"
#include "../Course/runner.h"
#include "playerclass.hh"

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
    void drawPlayerAgents(Playerclass &p);
    void drawItem(mapItem* item);
    void showHand();
    void sendAgentTo(LocationItem* loc);

    void enablePlayerHand(std::shared_ptr<Interface::Player> player);

private:
    Ui::GameWindow *gameui;
    GameScene *mapScene;
    std::shared_ptr<Interface::Game> gameboard = nullptr;
    std::shared_ptr<Interface::Runner> courseRunner = nullptr;

    // Testing for hands
    std::map<std::shared_ptr<Interface::Player>, std::shared_ptr<PlayerHand>> hands_;
    std::map<std::shared_ptr<Interface::Player>, QGraphicsWidget> nuhands_;

    const std::vector<QString> paikat_ = {"Marketti", "Kirkko", "Taverna", "Kauppiaiden kilta", "Menomesta", "Salapaikka"};

    int current_round;
    std::shared_ptr<Interface::Player> player1 = nullptr;
    std::shared_ptr<Interface::Player> player2 = nullptr;

};

#endif // GAMEWINDOW_HH
