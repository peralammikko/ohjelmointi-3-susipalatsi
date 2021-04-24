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
    // This is a deprecated method which needs to be replaced or removed
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;


    void drawItem(mapItem* item);

    // Makes agents visible and places them in a "deployment zone" (?) a hand-like area where agents are
    // and where they can be dragged to locations
    // Also connects these agent's onMapItemMouse* events to this scene
    // TODO: better name showDeployableAgents ?
    void drawAgents(std::vector<agentItem*> &agents);

    // Draws a mapitem for every location (aka buildings or planets)
    // pre: there are locations stored in locvec
    // post: scene has locations drawn on scene
    void drawLocations(std::vector<std::pair<std::shared_ptr<Interface::Location>, std::vector<std::pair<QString, QString>>>> locationInformation);

    // Creates a hand area for player
    void initHands(std::shared_ptr<const Interface::Player> Player);

    // Adds an action card to a player's hand
    void addActionCardForPlayer(std::shared_ptr<const Interface::Player> player, std::shared_ptr<Interface::ActionCard> card);

    void turnInfo(std::shared_ptr<Interface::Player> &currentplayer);

    void resourceInfo(ResourceMap &rmap, ResourceMap &dmap);

    void initPlayerHandFor(std::shared_ptr<Interface::Player> player);

    // Returns map of player hands
    std::map<std::shared_ptr<const Interface::Player>, PlayerHand*> playerHands();

    // Sets the declaring mapitem in a visible place, and draw arrows from its original position to its targeted mapitem
    // The game will start waiting for the manual player to use an action card. Also sets up few mapitem variables which allow us to reset this ready-state.
    void prepareForAction(std::shared_ptr<Interface::ActionInterface> action, mapItem* declaringMapItem);

    // A declared action is forgotten and declaring MapItem is returned back to its home position
    void resetAction();

    // Returns every location item on the scene
    std::vector<LocationItem *> GetLocItems();

    // When the player has been changed, makes every item that does not belong to the player undraggable.
    // This is signaled by game-class
    // Also moves other player hands on the side as face-down versions with shrunken size
    void onPlayerChanged(std::shared_ptr<const Interface::Player> actingPlayer);

    // Shuffles locations
    void nextRound();

    ResourceMap getResMap();

    // Returns vector of map items which belong to the player. AI needs this.
    std::vector<agentItem *> getAgentItemsForPlayer(std::shared_ptr<Interface::Player> player);


signals:
    // Sent when an Agent has declared an action (eg. User has dragged it from hand to a planet) AND paid for the aciton card
    void actionDeclared(std::shared_ptr<Interface::ActionInterface> action);
public slots:
    // Recieved when a mapItem (Agent or a CardItem) is dragged and dropped and the item has recognized a valid action.
    // If the item is an agent card, it will start waiting for an action card, or until the action has been cancelled by dragging it again.
    // If the dropped item is a carditem on a waiting agent, the card is discard and logic is informed with actionDeclared signal
    void onActionDeclared(std::shared_ptr<Interface::ActionInterface> action, mapItem* declaringMapItem, bool resetting);

private slots:
    // These are cut content for highlighting (making things larger) as you mouse over them.
    // We are afraid that removing this will cause bad trouble so it is here for now
    void onMapItemMouseDragged(mapItem* mapitem);
    // Location Items have their dialogs opened
    void onLocationItemClicked(LocationItem * locItem);

private:
    std::weak_ptr<Interface::Game> game_;
    std::map<std::shared_ptr<const Interface::Player>, PlayerHand*> playerHands_;

    // When a manual player declares an action, the game waits for them to choose an action card to discard.
    std::shared_ptr<Interface::ActionInterface> declaredAction_;
    mapItem* declaringMapItem_ = nullptr;

    // changes state of cards in handCards_ to show and arranges them nicely as a hand centered in handAnchorCoords_
    // also connects drag drop signals with those carditems
    void showHandCards();

    // Shuffles locationItems_, and also makes each location item know its new neighbour
    void shuffleLocationItems();
    // Places locations in a spherical rotation around center of the scene
    void rearrangeLocationItems();

    // Refreshes location items so that each location has a neighbour
    void resetLocationNeighbours();

    std::vector<LocationItem*> locationItems_;

    // "Arrows" (or lines!) that indicate from where agent is coming from and where it is heading
    SceneArrow* arrow1_;
    SceneArrow* arrow2_;

    ResourceMap resMap_;
    ResourceMap demandsMap_;

    QPointF solarsystemCenter_;
    QPointF const activePlayerHandAnchor_ = QPointF(150,650);
    QPointF const waitingPlayerHandAnchor_ = QPointF(20,20);
    int const SPACEBETWEENHANDS = 200;

    std::shared_ptr<Interface::Player> playerInTurn_ = nullptr;

    PopupDialog* clickDialog = nullptr;
};

#endif // GAMESCENE_HH
