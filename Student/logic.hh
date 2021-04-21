#ifndef LOGIC_HH
#define LOGIC_HH
#include <QObject>
#include <memory>

#include "gamewindow.hh"
#include "ui_gamewindow.h"


#include "gamescene.hh"
#include <cmath>

//#include "mapitem.hh"
//#include "locationitem.hh"

//#include "agent.hh"
//#include "agentitem.hh"
//#include "actioncard.hh"

#include "../Course/runner.h"
#include "../Course/game.h"
#include "../Course/player.h"

#include "../Course/actioninterface.h"
#include "../Course/controlinterface.h"
#include "../Course/manualcontrol.h"

class ActionCard;

class Logic : public QObject
{
    Q_OBJECT
public :
    Logic(std::shared_ptr<Interface::Runner> runner, std::shared_ptr<Interface::Game> game, GameScene *gameScene);
    ~Logic();

    void doTheRunning();
    // Attempting to move almost everything in mainwindow which is related to game rule intialisation here
    void launchGame();
    void createLocations();
    void rewardResources();
    void infoResourceMaps(ResourceMap &rmap, ResourceMap &dmap);

public slots:
    void actionSelected(std::shared_ptr<Interface::ActionInterface> action);
    // tell the game scene to switch player
    void onPlayerChanged(std::shared_ptr<const Interface::Player> actingPlayer);

    // sent by runner, connected in setup
    void onActionPerformed(std::shared_ptr<const Interface::Player> player, std::shared_ptr<Interface::ActionInterface> action);

private:
    void setNextAction();

    std::shared_ptr<Interface::Runner> runner_;
    std::shared_ptr<Interface::Game> game_;
    std::shared_ptr<GameScene> gameScene_;

    // Honestly no clue yet
    std::shared_ptr<Interface::ControlInterface> ctrl_;
    std::shared_ptr<Interface::ManualControl> manualCtrl_;
    std::shared_ptr<Interface::ActionInterface> action_;

    std::shared_ptr<const Interface::Player>  actingPlayer_;
    int current_turn = 1;

    ResourceMap resMap_;
    ResourceMap demandsMap_;
    
    // Holds info on players and their currency
    std::map<std::shared_ptr<Interface::Player>, int> playerWallets_;

    // Holds info on influence gained from locations by players
    // std::map<std::shared_ptr<Interface::Player>, std::map<std::shared_ptr<Interface::Location>, int>> playerInfluenceMap_;


};

#endif // LOGIC_HH
