#include "gamescene.hh"
#include "gamewindow.hh"
#include "agentitem.hh"
#include "mapitem.hh"
#include "locationitem.hh"
#include <cmath>
#include "carditem.hh"
#include "popupdialog.hh"

#include "../Course/game.h"


// RUNNER TESTING


// required for signaling??
#include <QObject>

GameScene::GameScene(QWidget *parent, std::weak_ptr<Interface::Game> game) : QGraphicsScene(parent), game_(game)
{

}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "mouse pos on click:" <<event->scenePos();
    update();
    QGraphicsScene::mousePressEvent(event);
}

void GameScene::drawLocations(std::vector<std::shared_ptr<Interface::Location>> &locvec)
{
    std::shared_ptr<Interface::Location> currentLocation = nullptr;

    // Piirretään rakennukset "ympyrän" kehälle
    const int xCenter = this->width()/2;
    const int yCenter = this->height()/2;

    // Needs more elegant implementation, like a global constant and a scaling value
    const int radius = 300;
    // by default we have 6 locations
    int locationCount = locvec.size();
    const int degree = 360 / locationCount;

    for (int i = 0; i < locationCount; i++) {
        currentLocation = locvec.at(i);
        LocationItem* locItem = new LocationItem(currentLocation, i);
        connect(locItem, &LocationItem::locationItemPressed, this, &GameScene::onLocationItemClicked);

        // Geometrinen sijainti kehällä
        float angleDeg = degree * i;
        float angleRad = angleDeg * M_PI / 180;
        float x = xCenter + radius * std::cos(angleRad);
        float y = yCenter + radius * std::sin(angleRad);

        drawItem(locItem);
        locItem->setParent(this);
        locItem->setPos(QPointF(x,y));
    }
}

void GameScene::drawItem(mapItem *item)
{
    addItem(item);
}

void GameScene::drawAgents(std::vector<agentItem*> &agents)
{
    if (!oneHand_){
        oneHand_ = new PlayerHand(this, playerInTurn_);
    }
    for (unsigned int i = 0; i < agents.size(); i++) {
        agentItem* current = agents.at(i);
        //std::shared_ptr<Interface::AgentInterface> agent = current->getObject(); // What is this for ?
        current->show();
        connect(current, &mapItem::mapItemMouseDragged, this, &GameScene::onMapItemMouseDragged);
        connect(current, &mapItem::mapItemMouseReleased, this, &GameScene::onMapItemMouseDropped);
        current->setPos(300+current->boundingRect().width()*i, 300);
        oneHand_->addMapItem(current);
        //oneHand_->r;
    }
}

void GameScene::hideAgents(std::vector<agentItem *> &agents)
{
    for (unsigned int i = 0; i < agents.size(); i++) {
        agentItem* current = agents.at(i);
        current->hide();

        disconnect(current, &mapItem::mapItemMouseDragged, this, &GameScene::onMapItemMouseDragged);
        disconnect(current, &mapItem::mapItemMouseReleased, this, &GameScene::onMapItemMouseDropped);
        //current->setPos(300+current->boundingRect().width()*i,300);
    }
}

void GameScene::initPlayerHandFor(std::shared_ptr<Interface::Player> player)
{
    playerHands_.insert(std::make_pair(player, new PlayerHand(this, player)));
}

void GameScene::createHandCards(std::vector<std::shared_ptr<Interface::CardInterface>> cards)
{
    //oneHand_ = new PlayerHand(this, playerInTurn_);
    this->addItem(oneHand_);
    oneHand_->setY(400);
    for (unsigned int i = 0; i < cards.size(); ++i) {
        std::shared_ptr<Interface::CardInterface> carddata = cards.at(i);
        CardItem *carditem = new CardItem(carddata, this);
        this->addItem(carditem);
        oneHand_->addMapItem(carditem);
        //handCards_.push_back(carditem);

        connect(carditem, &mapItem::mapItemMouseDragged, this, &GameScene::onMapItemMouseDragged);
        connect(carditem, &mapItem::mapItemMouseReleased, this, &GameScene::onMapItemMouseDropped);
    }

}


void GameScene::turnInfo(int turn, std::shared_ptr<Interface::Player> currentplayer)
{
    turn_ = turn;
    playerInTurn_ = currentplayer;
}

void GameScene::resourceInfo(AreaResources &rmap)
{
    resMap_ = rmap;
}


void GameScene::onMapItemMouseDragged(mapItem* mapitem)
{
    // TODO: Discuss with game logic and highlight/scale up items that are valid targets
    // ie. when agent is hovered on a building, it scales the closest building up a little, but not carditems or other agentitems.
    // Get every item under cardboundaries
    QList<QGraphicsItem*> items = mapitem->collidingItems();

    int count =0;
    for (int i = 0; i < items.size(); ++i)
    {
        count +=1;
        if (items.at(i) != mapitem) {
            // Followin allows us to get ANY type of interaface data under the rect
            // todo: prettier class type checking
             LocationItem* location = dynamic_cast<LocationItem*>(items.at(i));
             if (location != nullptr)
             {
                // so it is a location
             } else {
                agentItem* agent = dynamic_cast<agentItem*>(items.at(i));
                if (agent != nullptr)
                {

                }
             }
        }
    }
}

void GameScene::onMapItemMouseDropped(mapItem* mapitem)
{
    // This used to have most of the actioninterface stuff but it might not be needed anymore
}

void GameScene::onLocationItemClicked(LocationItem* locItem)
{
    CommonResource res = resMap_.at(locItem->getObject());
    int BV = locItem->getBasevalue();
    PopupDialog* clickDialog = new PopupDialog(locItem->getObject(), BV, res, playerInTurn_);
    clickDialog->show();

}

void GameScene::onActionDeclared(std::shared_ptr<Interface::ActionInterface> action)
{
    qDebug() << "Action declared, signal recieved gamescene";
    emit actionDeclared(action);
    oneHand_->rearrange();
}
