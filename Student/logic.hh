#ifndef LOGIC_HH
#define LOGIC_HH
#include <QObject>
#include <memory>

#include "gamewindow.hh"
#include "ui_gamewindow.h"
#include "mapitem.hh"
#include "locationitem.hh"
#include "agent.hh"
#include "agentitem.hh"
#include "gamescene.hh"
#include <cmath>

#include "../Course/runner.h"
#include "../Course/game.h"
#include "../Course/player.h"

#include "../Course/actioninterface.h"
#include "../Course/controlinterface.h"


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


public slots:
    void actionSelected(std::shared_ptr<Interface::ActionInterface> action);
    // tell the game scene to switch player
    void playerChanged(std::shared_ptr<const Interface::Player> actingPlayer) const;
private:
    void setNextAction();

    std::shared_ptr<Interface::Runner> runner_;
    std::shared_ptr<Interface::Game> game_;
    std::shared_ptr<GameScene> gameScene_;

    // Honestly no clue yet
    std::shared_ptr<Interface::ControlInterface> ctrl_;
    std::shared_ptr<Interface::ManualControl> manualCtrl_;
    std::shared_ptr<Interface::ActionInterface> action_;

    std::shared_ptr<Interface::Player>  actingPlayer_;

};

#endif // LOGIC_HH
