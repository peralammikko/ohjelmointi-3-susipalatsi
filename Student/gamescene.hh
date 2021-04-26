#ifndef GAMESCENE_HH
#define GAMESCENE_HH

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QList>
#include <cmath>

#include "game.h"
#include "agentitem.hh"
#include "locationitem.hh"
#include "carditem.hh"
#include "playerhand.hh"
#include "scenearrow.hh"
#include "../Course/deckinterface.h"
#include "actioncard.hh"
#include "popupdialog.hh"

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GameScene(QWidget* parent, std::weak_ptr<Interface::Game> game);

    ~GameScene();

    /**
     * @brief Makes agents visible and places them in a hand-like deployment zone
     * @param agents: Drawable AgentItem objects for each player
     * @pre Agents have been properly initialized and distributed
     */
    void drawAgents(std::vector<agentItem*> &agents);

    /**
     * @brief Draws game's locations to scene and initializes them as LocationItem objects
     * @param locationInformation: vector of Locations and their relative information
     * @pre Vector contains Locations and their information
     * @post All locations are displayed on screen as LocationItem objects
     */
    void drawLocations(std::vector<std::pair<std::shared_ptr<Interface::Location>, std::vector<std::pair<QString, QString>>>> locationInformation);

    // Creates a hand area for player
    /**
     * @brief Creates a hand area for player to hold cards & agents
     * @param Player: Player in game
     */
    void initHands(std::shared_ptr<const Interface::Player> Player);

    // Adds an action card to a player's hand
    /**
     * @brief Adds an action card to players hand
     * @param player: Player in question
     * @param card: Action card to be added
     * @pre Action card is properly initialized and drawn
     * @post Player gets a new action card in his hand
     */
    void addActionCardForPlayer(std::shared_ptr<const Interface::Player> player, std::shared_ptr<Interface::ActionCard> card);

    /**
     * @brief Receives information about turns
     * @param currentplayer: player in turn
     */
    void turnInfo(std::shared_ptr<Interface::Player> &currentplayer);

    /**
     * @brief Receives information about resources in each location
     * @param rmap: Local collectable resources in each location
     * @param dmap: Initial demanded resource in each location
     */
    void resourceInfo(ResourceMap &rmap, ResourceMap &dmap);

    /**
     * @brief Initializes player hand for a player
     * @param player: Player in question
     * @pre Player exists
     * @post Player now has a hand
     */
    void initPlayerHandFor(std::shared_ptr<Interface::Player> player);

    /**
     * @brief Returns all players' hands
     * @return Map of player hands
     */
    std::map<std::shared_ptr<const Interface::Player>, PlayerHand*> playerHands();

    /**
     * @brief Sets the declaring mapitem in a visible place, and draw arrows from its original position to its targeted mapitem
     * The game will start waiting for the manual player to use an action card. Also sets up few mapitem variables which allow us to reset this ready-state.
     * @param action: Action performed by player
     * @param declaringMapItem: MapItem player has targeted
     */
    void prepareForAction(std::shared_ptr<Interface::ActionInterface> action, mapItem* declaringMapItem);

    /**
     * @brief A declared action is forgotten and declaring MapItem is returned back to its home position
     * @pre An action is perfomed
     * @post Action is reversed and player is back in time before said action
     */
    void resetAction();

    /**
     * @brief Returns every location item on the scene
     * @return Vector of LocationItem pointers
     */
    std::vector<LocationItem *> GetLocItems();

    /**
     * @brief When the player has been changed, makes every item that does not belong to the player undraggable. Signaled by Game class.
     * @param actingPlayer: Previous turn's player
     * @pre Player has ended his turn
     * @post Other players hands are moved away and shrunk from current players view
     */
    void onPlayerChanged(std::shared_ptr<const Interface::Player> actingPlayer);

    /**
     * @brief Shuffles game's locations around in what's called the Event phase
     */
    void nextRound();

    /**
     * @brief Return the games resource (collectible) resources per location
     * @return Map of locations and their local resources
     */
    ResourceMap getResMap();

    /**
     * @brief Returns vector of map items which belong to the player. AI needs this.
     * @param player: Player in question
     * @return Vector of agents owned by player
     * @pre Player owns agents
     */
    std::vector<agentItem *> getAgentItemsForPlayer(std::shared_ptr<Interface::Player> player);


signals:
    /**
     * @brief Sent when an Agent has declared an action (eg. User has dragged it from hand to a planet) AND paid for the aciton card
     * @param action
     */
    void actionDeclared(std::shared_ptr<Interface::ActionInterface> action);
public slots:

    /**
     * @brief Recieved when a mapItem (Agent or a CardItem) is dragged and dropped and the item has recognized a valid action.
     * @param action: Action performed
     * @param declaringMapItem: Targeted MapItem. If an agent card, scene will wait for an action card to be dragged or the action to be cancelled.
     * @param resetting: Check if action will be cancelled
     * @pre Player has MapItems to move around and perform actions upon
     * @post Dropped item is a carditem on a waiting agent, the card is discard and logic is informed with actionDeclared signal or action is cancelled
     */
    void onActionDeclared(std::shared_ptr<Interface::ActionInterface> action, mapItem* declaringMapItem, bool resetting);

private slots:
    // These are cut content for highlighting (making things larger) as you mouse over them.
    // We are afraid that removing this will cause bad trouble so it is here for now
    /**
     * @brief Neede for tracking which MapItem is being dragged.
     * @param mapitem: MapItem being dragged
     */
    void onMapItemMouseDragged(mapItem* mapitem);

    /**
     * @brief Clicking a location item opens a dialog for the player to inspect
     * @param locItem: Clicked location
     */
    void onLocationItemClicked(LocationItem * locItem);


private:
    std::weak_ptr<Interface::Game> game_;
    std::map<std::shared_ptr<const Interface::Player>, PlayerHand*> playerHands_;

    /**
     * @brief declaredAction_: When a manual player declares an action, the game waits for them to choose an action card to discard.
     */
    std::shared_ptr<Interface::ActionInterface> declaredAction_;
    mapItem* declaringMapItem_ = nullptr;

    /**
     * @brief Changes state of cards in handCards_ to show and arranges them nicely as a hand centered in handAnchorCoords_
     * Connects drag drop signals with those carditems
     */
    void showHandCards();

    /**
     * @brief Shuffles locationItems_, and also makes each location item know its new neighbour
     * @pre Locations exist and have neighbours
     */
    void shuffleLocationItems();

    /**
     * @brief Places locations in a spherical rotation around center of the scene
     */
    void rearrangeLocationItems();

    /**
     * @brief Refreshes location items so that each location has a neighbour
     */
    void resetLocationNeighbours();

    std::vector<LocationItem*> locationItems_;

    /**
     * @brief "Arrows" (or lines!) that indicate from where agent is coming from and where it is heading
     */
    SceneArrow* arrow1_;
    SceneArrow* arrow2_;

    /**
     * @brief Resource maps used to initialize locations and their resources (collectible & demanded)
     */
    ResourceMap resMap_;
    ResourceMap demandsMap_;

    QPointF solarsystemCenter_;
    QPointF const activePlayerHandAnchor_ = QPointF(150,650);
    QPointF const waitingPlayerHandAnchor_ = QPointF(20,20);
    int const SPACEBETWEENHANDS = 200;

    std::shared_ptr<Interface::Player> playerInTurn_ = nullptr;


    /**
     * @brief A custom QDialog spawned by clicking on locations
     */
    PopupDialog* clickDialog = nullptr;
};

#endif // GAMESCENE_HH
