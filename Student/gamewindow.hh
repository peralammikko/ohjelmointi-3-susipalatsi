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
#include "agentactioninterface.hh"
#include "passaction.hh"

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
    void listInfluence(std::shared_ptr<Interface::Player> &currentPlayer);


private slots:
    void on_passButton_clicked();
    // Displays the most recent action on the history tab
    void onActionPerformed(std::shared_ptr<const Interface::Player> player, std::shared_ptr<Interface::ActionInterface> action);

    void onPlayerChanged(std::shared_ptr<const Interface::Player> actingPlayer);

    // Adds history notation that event phase happened
    void onEnteringEventPhase();

    void getStartingInfo(std::vector<QString> playerNames, std::vector<int> gameSettings);

signals:
    void actionDeclared(std::shared_ptr<Interface::ActionInterface> action);
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

    std::vector<QString> playerNames_ = {};
    std::vector<int> gameSettings_ = {};

};

#endif // GAMEWINDOW_HH
