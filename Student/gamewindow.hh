#ifndef GAMEWINDOW_HH
#define GAMEWINDOW_HH

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QMessageBox>

#include "gamescene.hh"
#include "game.h"
#include "playerhand.hh"
#include "councilor.h"
#include "controlinterface.h"
#include "agentactioninterface.hh"
#include "passaction.hh"
#include "logic.hh"
#include "../Course/game.h"
#include "../Course/runner.h"
#include "commonresource.hh"
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

    /**
     * @brief Returns all locations in game in a vector
     * @return Vector consisting of shared pointers of Locations
     */
    const std::vector<std::shared_ptr<Interface::Location>> getLocations();

    /**
     * @brief setSize:
     * @param width:
     * @param height:
     */
    void setSize(int width, int height); // Poistoon?

    /**
     * @brief drawItem
     * @param item
     */
    void drawItem(mapItem* item); // Poistoon?
    /**
     * @brief showHand
     */
    void showHand(); // Poistoon?

    /**
     * @brief Updates the player stats for the player in turn
     * @pre Game has players and turns are being switched
     */
    void displayPlayerStats();

    /**
     * @brief Lists all the available agents for the current player
     * @param currentPlayer: player in turn
     * @pre Game has players and player has agents
     * @post All of players (available) agents are displayed in the side panel
     */
    void listAgents(std::shared_ptr<Interface::Player> &currentPlayer);

    /**
     * @brief Displays all the influences a player has on Locations
     * @param currentPlayer: player in turn
     */
    void listInfluence(std::shared_ptr<Interface::Player> &currentPlayer);

    /**
     * @brief Displays and lists score as council cards collected for all players
     */
    void listCouncilCards();

    /**
     * @brief Gives a brief explanation to players about the game's goal and objectives
     */
    void startingDialog();

public slots:
    /**
     * @brief onInterphaseRequested disables the gamescene and starts a timer, which on time-out calls onInterphaseTimeout
     * @param time time in mseconds
     */
    void onInterphaseRequested(int time);

private slots:
    /**
     * @brief Ends the turn for player
     * @pre Player who clicks it is in turn
     * @post Player can't interact for the rest of the turn and hand cards are discarded
     */
    void on_passButton_clicked();


    /**
     * @brief Displays the latest action a player has commited in the side panel
     * @param player: player in turn
     * @param action: action performed by player
     * @pre player has actions to perform, player is in turn
     * @post player's action is displayed in the History window in main window
     */
    void onActionPerformed(std::shared_ptr<const Interface::Player> player, std::shared_ptr<Interface::ActionInterface> action);

    /**
     * @brief Receives information about player's turn ending
     * @param actingPlayer: previous turn's player
     */
    void onPlayerChanged(std::shared_ptr<const Interface::Player> actingPlayer);


    /**
     * @brief Displays a message on the History window about Event phase happening.
     * @pre Event phase has happened (all players have ended their turn)
     * @post Message is displayed in the side panel History window
     */
    void onEnteringEventPhase();

    /**
     * @brief Receives player names and game settings from the starting screen
     * @param playerNames: names for all registered players
     * @param gameSettings: settings chosen by the player
     * @param bots: Number of AI opponents chosen by the player
     * @pre Starting screen has opened and the Start game has been pressed
     * @post Initializes vectors used for setting up the game
     */
    void getStartingInfo(std::vector<QString> playerNames, std::vector<int> gameSettings, int bots);

    void on_helpButton_clicked();

    /**
     * @brief onInterphaseTimeout re-enables graphicsView_
     */
    void onInterphaseTimeout();

signals:
    /**
     * @brief Sends information about player performing an action
     * @param action: Action performed by player (in this case Pass)
     */
    void actionDeclared(std::shared_ptr<Interface::ActionInterface> action);
private:
    Ui::GameWindow *gameui_;
    GameScene *gameScene_;
    std::shared_ptr<Interface::Game> game_ = nullptr;
    std::shared_ptr<Interface::Runner> courseRunner = nullptr;

    // Logic testing
    std::shared_ptr<Logic> logic_;

    std::unique_ptr<QTimer> gameTime_;   
    QTimer* interphaseTimer_ = nullptr;

    // Usefulness to be decided.
    std::map<std::shared_ptr<Interface::Player>, std::vector<agentItem*>> playerAgentItems_;

    // this variable stores drag and drop targe, ie. what is "under" a draggable card
    mapItem* targetedMapItem_;

    int current_round = 1;

    std::vector<QString> playerNames_ = {};
    std::vector<int> gameSettings_ = {};
    int bots_ = 0;
    int winCondition = 3;

    QMessageBox* infoBox_;

};

#endif // GAMEWINDOW_HH
