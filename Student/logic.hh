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
    Logic(std::shared_ptr<Interface::Runner> runner, std::shared_ptr<Interface::Game> game);
    ~Logic();



    void doTheRunning();

public slots:
    void actionSelected(std::shared_ptr<Interface::ActionInterface> action);
    // tell the game scene to switch player
    void playerChanged(std::shared_ptr<const Interface::Player> actingPlayer) const;
private:
    void setNextAction();

    std::shared_ptr<Interface::Runner> runner_;
    std::shared_ptr<Interface::Game> game_;

    // Honestly no clue yet
    std::shared_ptr<Interface::ControlInterface> ctrl_;
    std::shared_ptr<Interface::ManualControl> manualCtrl_;
    std::shared_ptr<Interface::ActionInterface> action_;

    std::shared_ptr<Interface::Player>  actingPlayer_;

};

#endif // LOGIC_HH
