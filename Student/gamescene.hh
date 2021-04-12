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
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void drawLocations(std::vector<std::shared_ptr<Interface::Location>> &locvec);
    void drawItem(mapItem* item);
    void drawAgents(std::vector<agentItem*> &agents);

    // creates nice carditem for each cardinterface in vector, calls showHandCards
    void createHandCards(std::vector<std::shared_ptr<Interface::CardInterface>> cards);

public slots:
    void onCardDragged(CardItem* card);
    void onCardDropped(CardItem* card);

private:
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
