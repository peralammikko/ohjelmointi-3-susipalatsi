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
#include "playerhand.hh"
#include "councilor.h"
#include "controlinterface.h"

// Logic testing
#include "logic.hh"

#include "../Course/game.h"
#include "../Course/runner.h"

#include "commonresource.hh"
#include "gamerunner.hh"
#include "random.h"

#include <QTimer>

namespace Ui {
    class GameWindow;

}

class Logic;
class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();
    const std::vector<std::shared_ptr<Interface::Location>> getLocations();
    void setSize(int width, int height);
    void showAgentsForPlayer(std::shared_ptr<Interface::Player> player);
    void drawItem(mapItem* item);
    void showHand();

    void setupPlayerStash();
    void displayPlayerStats();

    void initAreaResources();

    void rewardResources();

    void listAgents(std::shared_ptr<Interface::Player> &currentPlayer);


private slots:
    void on_passButton_clicked();
    void onPlayerChanged(std::shared_ptr<const Interface::Player> actingPlayer);

private:

    Ui::GameWindow *gameui_;
    GameScene *gameScene_;
    std::shared_ptr<Interface::Game> game_ = nullptr;
    std::shared_ptr<GameRunner> courseRunner = nullptr;

    // Logic testing
    std::shared_ptr<Logic> logic_;

    std::unique_ptr<QTimer> gameTime_;

    // Usefulness to be decided.
    std::map<std::shared_ptr<Interface::Player>, std::vector<agentItem*>> playerAgentItems_;

    // this variable stores drag and drop targe, ie. what is "under" a draggable card
    mapItem* targetedMapItem_;

    int current_round = 1;

};

#endif // GAMEWINDOW_HH
