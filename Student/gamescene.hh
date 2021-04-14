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

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GameScene(QWidget* parent = nullptr);

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
    void hideAgents(std::vector<agentItem*> &agents);

    // Draws a mapitem for every location (aka buildings or planets)
    // pre: there are locations stored in locvec
    // post: scene has locations drawn on scene
    void drawLocations(std::vector<std::shared_ptr<Interface::Location>> &locvec);

    // creates nice carditem for each cardinterface in vector
    // Post: carditems spawned and calls showHandCards
    void createHandCards(std::vector<std::shared_ptr<Interface::CardInterface>> cards);

    std::shared_ptr<Interface::Game> connectGameboard();

    void turnInfo(int turn, std::shared_ptr<Interface::Player> currentplayer);

    void resourceInfo(AreaResources &rmap);


private slots:
    void onMapItemMouseDragged(mapItem *mapitem);
    void onMapItemMouseDropped(mapItem* mapitem);

private:
    mapItem* targetedMapItem_;

    mapItem* selectedLocation = nullptr;
    agentItem* selectedAgent = nullptr;

    int turn_ = 0;
    std::shared_ptr<Interface::Player> playerInTurn_ = nullptr;

    std::vector<CardItem*> handCards_;
    std::pair<int, int> handAnchorCoords_;
    int handCardPadding_;

    // changes state of cards in handCards_ to show and arranges them nicely as a hand centered in handAnchorCoords_
    // also connects drag drop signals with those carditems
    void showHandCards();
    std::shared_ptr<Interface::Game> gameboard_ = nullptr;

    AreaResources resMap_ = {};



};

#endif // GAMESCENE_HH
