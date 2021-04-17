#include "gamescene.hh"
#include "gamewindow.hh"
#include "agentitem.hh"
#include "mapitem.hh"
#include "locationitem.hh"
#include <cmath>
#include "carditem.hh"
#include "popupdialog.hh"
#include "agentdialog.hh"

#include "../Course/game.h"

// required for signaling??
#include <QObject>

GameScene::GameScene(QWidget *parent, std::weak_ptr<Interface::Game> game) : QGraphicsScene(parent), game_(game), handAnchorCoords_(std::make_pair(0, 400)), handCardPadding_(5)
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
        LocationItem* locItem = new LocationItem(currentLocation);
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
    for (unsigned int i = 0; i < agents.size(); i++) {
        agentItem* current = agents.at(i);
        current->show();
        connect(current, &mapItem::mapItemMouseDragged, this, &GameScene::onMapItemMouseDragged);
        connect(current, &mapItem::mapItemMouseReleased, this, &GameScene::onMapItemMouseDropped);
        current->setPos(300+current->boundingRect().width()*i, 300);
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

void GameScene::createHandCards(std::vector<std::shared_ptr<Interface::CardInterface>> cards)
{
    for (unsigned int i = 0; i < cards.size(); ++i) {
        std::shared_ptr<Interface::CardInterface> carddata = cards.at(i);
        CardItem *carditem = new CardItem(carddata, this);
        carditem->setParent(this);
        // adds card to the scene
        this->addItem(carditem);
        carditem->hide();
        handCards_.push_back(carditem);

        connect(carditem, &mapItem::mapItemMouseDragged, this, &GameScene::onMapItemMouseDragged);
        connect(carditem, &mapItem::mapItemMouseReleased, this, &GameScene::onMapItemMouseDropped);
    }
    showHandCards();
}

void GameScene::turnInfo(int turn, std::shared_ptr<Interface::Player> currentplayer)
{
    turn_ = turn;
    playerInTurn_ = currentplayer;
}

void GameScene::resourceInfo(ResourceMap &rmap)
{
    resMap_ = rmap;
}


void GameScene::showHandCards()
{
    float widthtotal = 0.0;
    int xStart;
    float widthPerCard;

    int count = handCards_.size();
    if (count) {
        // Calculate total width of the hand
        for (int i = 0; i < count; ++i) {
            handCards_.at(i)->show();
            widthtotal += handCards_.at(i)->boundingRect().width();
        }
        // Gets the new coords for cards based on hand width
        widthPerCard = (widthtotal + handCardPadding_*count) / count;
        xStart =  handAnchorCoords_.first - (widthtotal / 2);
        for (int i = 0; i < count; ++i) {
            handCards_.at(i)->setParent(this);
            int x = (xStart + widthPerCard*i);
            handCards_.at(i)->setPos(x, handAnchorCoords_.second);
        }
    }
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
    // TODO: implement logic to see if the move is legal
     // check if the game is still going
     if (!game_.lock())
     {
         qDebug() << "Fatal error: tried to move map items while there is no game";
         // TODO: close program?
         return;
     }
     // For now we just check if agent has been dropped on a building
     // In a real scenario we can emit a signal which contains dropped mapitem and a vector of colliding items
     // so they are handled elsewhere
     agentItem* aitem = dynamic_cast<agentItem*>(mapitem);
     if (aitem != nullptr)
     {
         QList<QGraphicsItem*> items = mapitem->collidingItems();
         for (int i = 0; i < items.size(); ++i)
         {

             if (items.at(i) != mapitem) {
                 // Followin allows us to get ANY type of interaface data under the rect
                 // todo: prettier class type checking
                  LocationItem* lItem = dynamic_cast<LocationItem*>(items.at(i));
                  if (lItem != nullptr)
                  {
                      if (canMoveAgent(lItem, aitem))
                      {
                          moveAgent(lItem, aitem);
                          break;
                      }
                  }
             }
         }

     }
    mapitem->goHome();

}


bool GameScene::canMoveAgent(LocationItem* newLocItem, agentItem* aItem)
{
    if (!game_.lock())
    {
        //todo: throw a real error
        qDebug() << "Fatal error!";
        return false;
    }
    std::shared_ptr<Interface::AgentInterface> aInterface = aItem->getObject();
    std::shared_ptr<Interface::Location> newPlacInterface = newLocItem->getObject();
    std::shared_ptr<Interface::Location> oldPlacInterface = aInterface->placement().lock();
    if (!oldPlacInterface)
    {
       // TODO: emit some signal to check viability
       //
       return true;
    } else {
       // oldPlacInterface->removeAgent(aInterface);

        auto locs = game_.lock()->locations();
        // get itarator of the agent and the itarator of the targeted location in game's locvec
        auto targetIt = std::find(locs.begin(), locs.end(), newPlacInterface);
        auto startingIt = std::find(locs.begin(), locs.end(), oldPlacInterface);
        if (startingIt != locs.end() and targetIt != locs.end())
        {
              // Calculate the distances between locations
              long dist = abs(std::distance(startingIt, targetIt));
              // TODO: maybe implement movements which are larger than one
              if ( dist == 1 or dist == locs.size()-1 )
              {
                  // TODO: maybe additional pooling
                  return true;
              }
        }
    }
    return false;
}

void GameScene::moveAgent(LocationItem* newLocItem, agentItem* aItem)
{
    std::shared_ptr<Interface::AgentInterface> aInterface = aItem->getObject();
    std::shared_ptr<Interface::Location> newPlacInterface = newLocItem->getObject();
    std::shared_ptr<Interface::Location> oldPlacInterface = aInterface->placement().lock();

    // Removes agent from its previous location, sends the agent to new location and sets new "home coords"
    if (oldPlacInterface){
        oldPlacInterface->removeAgent(aInterface);
    }
    newPlacInterface->sendAgent(aInterface);
    aItem->setParentItem(newLocItem);
    aItem->setHome(QPointF(0,0));
}


void GameScene::onLocationItemClicked(LocationItem* locItem)
{
    Interface::CommonResource res = resMap_.at(locItem->getObject());
    int BV = locItem->getBasevalue();
    PopupDialog* clickDialog = new PopupDialog(locItem->getObject(), BV, res, playerInTurn_);
    clickDialog->show();

}
