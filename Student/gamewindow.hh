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
    void drawPlayerAgents(std::shared_ptr<Interface::Player> &player);
    void drawItem(mapItem* item);
    void showHand();

    void enablePlayerHand(std::shared_ptr<Interface::Player> player);
    void sendAgentTo(const std::shared_ptr<Interface::Location> &loc, std::shared_ptr<Interface::Player> &player);

    void spawnAgent(std::shared_ptr<Interface::Player> &player);
    std::shared_ptr<Interface::Player> getPlayerObject();
    std::vector<agentItem *> getAgents(std::shared_ptr<Interface::Player> &player);
    std::shared_ptr<Interface::Player> getPlayerInTurn();

    void changeTurn();

    void listAgents(std::shared_ptr<Interface::Player> player);


private slots:
    void on_passButton_clicked();

private:
    Ui::GameWindow *gameui;
    GameScene *mapScene;
    std::shared_ptr<Interface::Game> gameboard = nullptr;
    std::shared_ptr<Interface::Runner> courseRunner = nullptr;

    // Testing for hands
    std::map<std::shared_ptr<Interface::Player>, std::shared_ptr<PlayerHand>> hands_;
    std::map<std::shared_ptr<Interface::Player>, QGraphicsWidget> playerhands_;

    const std::vector<QString> paikat_ = {"Marketti", "Kirkko", "Taverna", "Kauppiaiden kilta", "Menomesta", "Salapaikka"};

    int current_round = 0;
    bool gameOver = false;
    std::shared_ptr<Interface::Player> player1 = nullptr;
    std::shared_ptr<Interface::Player> player2 = nullptr;

    std::shared_ptr<Interface::Player> playerInTurn = nullptr;
    std::map<std::shared_ptr<Interface::Player>, std::vector<agentItem*>> playerAgents_;



    // this variable stores drag and drop targe, ie. what is "under" a draggable card
    mapItem* targetedMapItem_;

};

#endif // GAMEWINDOW_HH
