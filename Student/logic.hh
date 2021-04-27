#ifndef LOGIC_HH
#define LOGIC_HH
#include <QObject>
#include <memory>

#include "gamewindow.hh"


#include "gamescene.hh"
#include <cmath>

#include "actioncard.hh"

#include "../Course/runner.h"
#include "../Course/game.h"
#include "../Course/player.h"

#include "../Course/actioninterface.h"
#include "../Course/controlinterface.h"
#include "../Course/manualcontrol.h"

class ActionCard;

/**
 * @file
 * @brief Logic handles gamerules, victory check and roundswitching
 */


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

    /**
     * @brief onActionPerformed sent by runner, connected in setup
     * @pre the action is legal
     * @pre connected to runner
     * @param player
     * @param action
     */
    void onActionPerformed(std::shared_ptr<const Interface::Player> player, std::shared_ptr<Interface::ActionInterface> action);

    /**
     * @brief onInterphaseTimeout Calls the game to move on to the next player, which in turn signals onPlayerChanged (if the game is still ongoing)
     */
    void onInterphaseTimeout();

signals:
    /**
     * @brief enteredEventPhase when every player has passed the game proceeds to the event phase which makes history list widget update
     * @pre Connected to gameswindow
     */
    void enteredEventPhase();

    /**
     * @brief readyToRewardResources signals the resourcedealer to deal resources for every agent
     * @pre connected to a resourcedealer
     */
    void readyToRewardResources();

    /**
     * @brief enteringNextRound informs the gamescene that the game is ready for the next round, and tells the gamescene to shuffle locations
     * @pre connected to gamescene
     */
    void enteringNextRound();

    /**
     * @brief requestInterphase tells the GameWindow to disable graphicsview for a while
     * @param time the amount of time to be disabled in ms
     * @pre the gamewindow must exist, and the gamewindow must be connected to this signal
     */
    void requestInterphase(int time);

    /**
     * @brief onWinnersFound informs the gamewindow that the game is now over
     * @param winnerset
     * @pre connected to gamewindow
     */
    void onWinnersFound(std::set<std::shared_ptr<Interface::Player>> winnerset);

private:

    void setNextAction();

    /**
     * @brief reshuffleLocationDecks if the deck is empty, tries to reshuffle location's discard pile to main deck
     * @pre locations have discards and decks initialized
     * @post empty decks are now filled with cards from discard pile and shuffled, discards empty
     */
    void reshuffleLocationDecks();

    std::shared_ptr<Interface::Runner> runner_;
    std::shared_ptr<Interface::Game> game_;

    std::shared_ptr<Interface::ControlInterface> ctrl_;
    std::shared_ptr<Interface::ManualControl> manualCtrl_;
    std::shared_ptr<Interface::ActionInterface> action_;

    std::shared_ptr<const Interface::Player>  actingPlayer_;
    int current_turn = 1;

    int winCondition_ = 3;


};

#endif // LOGIC_HH
