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

#include "game.h"
#include "agentitem.hh"
#include "locationitem.hh"
#include "mapitem.hh"
#include "carditem.hh"


#include "../Course/manualcontrol.h"

class LocationItem;


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
    void drawLocations(std::vector<std::shared_ptr<Interface::Location>> &locvec);

    // creates nice carditem for each cardinterface in vector
    // Post: carditems spawned and calls showHandCards
    void createHandCards(std::vector<std::shared_ptr<Interface::CardInterface>> cards);

    void turnInfo(int turn, std::shared_ptr<Interface::Player> currentplayer);

    void resourceInfo(AreaResources &rmap);

signals:
    void actionDeclared(std::shared_ptr<Interface::ActionInterface> action);

public slots:
    void onActionDeclared(std::shared_ptr<Interface::ActionInterface> action);
private slots:
    void onMapItemMouseDragged(mapItem* mapitem);
    void onMapItemMouseDropped(mapItem* mapitem);
    void onLocationItemClicked(LocationItem * locItem);


private:
    // These are deprecated for now and waiting for safe removal
    mapItem* targetedMapItem_;
    mapItem* selectedLocation = nullptr;
    agentItem* selectedAgent = nullptr;

    int turn_ = 0;
    std::shared_ptr<Interface::Player> playerInTurn_ = nullptr;

    // currently displayed card items that are in a player's hand
    std::vector<CardItem*> handCards_;
    // the point which determines where hand is drawn
    std::pair<int, int> handAnchorCoords_;
    // Gap between card items in hand
    int handCardPadding_;

    std::weak_ptr<Interface::Game> game_;

    // changes state of cards in handCards_ to show and arranges them nicely as a hand centered in handAnchorCoords_
    // also connects drag drop signals with those carditems
    void showHandCards();


    // Sees if aItem can move to newLocation
    bool canMoveAgent(LocationItem* newLocation, agentItem* aItem);
    // Moves agent to a new location
    void moveAgent(LocationItem* newLocItem, agentItem* aItem);

    AreaResources resMap_ = {};
};

#endif // GAMESCENE_HH
