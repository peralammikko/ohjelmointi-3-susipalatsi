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
#include "mapitem.hh"
#include "carditem.hh"

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GameScene(QWidget* parent = nullptr);

    // This is a deprecated method which needs to be replaced or removed
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    // Draws mapitems
    // pre: nothing
    // post: scene has one more extra mapitem displayed
    void drawItem(mapItem* item);

    // Draws a mapitem for every location (aka buildings or planets)
    // pre: there are locations stored in locvec
    // post: scene has locations drawn on scene
    void drawLocations(std::vector<std::shared_ptr<Interface::Location>> &locvec);

    // creates nice carditem for each cardinterface in vector
    // Post: carditems spawned and calls showHandCards
    void createHandCards(std::vector<std::shared_ptr<Interface::CardInterface>> cards);

    // Gets cursor position and displays items under cursors on console
    // TODO: Choose one of thse MapItems as a target. Targeting should be based on CardItem's CardInterFace rules.
    void onCardDragged(CardItem *card);

    // TODO: If there is a valid MapItem stored in this class' pointer, do Card's action where target MapItem is the target.
    void onCardDropped(CardItem* card);

private:
    mapItem* targetedMapItem_;

    mapItem* selectedLocation = nullptr;
    agentItem* selectedAgent = nullptr;

    std::vector<CardItem*> handCards_;
    std::pair<int, int> handAnchorCoords_;
    int handCardPadding_;

    // changes state of cards in handCards_ to show and arranges them nicely as a hand centered in handAnchorCoords_
    // also connects drag drop signals with those carditems
    void showHandCards();
};

#endif // GAMESCENE_HH
