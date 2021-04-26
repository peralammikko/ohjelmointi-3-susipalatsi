#ifndef LOGIC_HH
#define LOGIC_HH
#include <QObject>
#include <memory>

#include "gamewindow.hh"
//#include "ui_gamewindow.h"


#include "gamescene.hh"
#include <cmath>

//#include "mapitem.hh"
//#include "locationitem.hh"

//#include "agent.hh"
//#include "agentitem.hh"
#include "actioncard.hh"

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
    Logic(std::shared_ptr<Interface::Runner> runner, std::shared_ptr<Interface::Game> game);
    ~Logic();

    void doTheRunning();
    // Attempting to move almost everything in mainwindow which is related to game rule intialisation here
    void launchGame();
    void createLocations();
    std::set<std::shared_ptr<Interface::Player>> checkWin(std::vector<std::shared_ptr<Interface::Player>> players);

public slots:
    void onActionDeclared(std::shared_ptr<Interface::ActionInterface> action);
    // tell the game scene to switch player
    void onPlayerChanged(std::shared_ptr<const Interface::Player> actingPlayer);

    // If the deck has ran out, this method would reshuffle the discards back in the deck. But it will not.
    void deckChanged(std::shared_ptr<const Interface::Location> location) const;

    // sent by runner, connected in setup
    void onActionPerformed(std::shared_ptr<const Interface::Player> player, std::shared_ptr<Interface::ActionInterface> action);

    /**
     * @brief onInterphaseTimeout Calls the game to move on to the next player, which in turn signals onPlayerChanged (if the game is still ongoing)
     */
    void onInterphaseTimeout();

signals:
    void enteredEventPhase();

    void readyToRewardResources();

    void enteringNextRound();

    /**
     * @brief requestInterphase tells the GameWindow to disable graphicsview for a while
     * @param time the amount of time to be disabled in ms
     * @pre the gamewindow must exist, and the gamewindow must be connected to this signal
     */
    void requestInterphase(int time);

private:
    void setNextAction();
    bool thereIsWinner();

    std::shared_ptr<Interface::Runner> runner_;
    std::shared_ptr<Interface::Game> game_;

    // Honestly no clue yet
    std::shared_ptr<Interface::ControlInterface> ctrl_;
    std::shared_ptr<Interface::ManualControl> manualCtrl_;
    std::shared_ptr<Interface::ActionInterface> action_;

    std::shared_ptr<const Interface::Player>  actingPlayer_;
    int current_turn = 1;

    int winCondition_ = 3;


};

#endif // LOGIC_HH
