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

    // This is a deprecated method which needs to be replaced or removed
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    // //////////////////////////////////
    // TODO: maybe merge drawItem, drawLocations, createHand and the like?
    // Every map item could just have on constructor method to keep thing simple
    // //////////////////////////////////
    // Draws mapitems
    // pre: nothing
    // post: scene has one more extra mapitem displayed
    // TODO:Is this useless?
    void drawItem(mapItem* item);

    // Makes agents visible and places them in a "deployment zone" (?) a hand-like area where agents are
    // and where they can be dragged to locations
    // Also connects these agent's onMapItemMouse* events to this scene
    // TODO: better name showDeployableAgents ?
    void drawAgents(std::vector<agentItem*> &agents);

    // Does the opposite of drawAgents, and hides all agents in vector and disconnects mouse-events
    void hideAgents( std::vector<agentItem*> &agents);

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

    // Returns every location item (planet)
    std::vector<LocationItem *> GetLocItems();

    // When the player has been changed, makes every item that does not belong to the player undraggable.
    // This is signaled by game-class
    // Also moves other player hands on the side as face-down versions with shrunken size
    void onPlayerChanged(std::shared_ptr<const Interface::Player> actingPlayer);

    // Shuffles locations
    void nextRound();

    ResourceMap getResMap();


signals:
    void actionDeclared(std::shared_ptr<Interface::ActionInterface> action);
public slots:
    void onActionDeclared(std::shared_ptr<Interface::ActionInterface> action, mapItem* declaringMapItem, bool resetting);


private slots:
    void onMapItemMouseDragged(mapItem* mapitem);
    void onLocationItemClicked(LocationItem * locItem);

private:
    std::map<std::shared_ptr<const Interface::Player>, PlayerHand*> playerHands_;
    std::weak_ptr<Interface::Game> game_;

    // When a manual player declares an action, the game waits for them to choose an action card to discard.
    std::shared_ptr<Interface::ActionInterface> declaredAction_;

    mapItem* declaringMapItem_;

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
