#include "gamescene.hh"
#include "gamewindow.hh"
#include "agentitem.hh"
#include "mapitem.hh"
#include "locationitem.hh"
#include <cmath>
#include "carditem.hh"
#include "popupdialog.hh"
#include "agentdialog.hh"
#include "game.h"

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
        Interface::CommonResource localRes = resMap_.at(currentLocation);
        locItem->setLocalResource(localRes);
        Interface::CommonResource demandRes = demandsMap_.at(currentLocation);
        locItem->setDemandedResource(demandRes);

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


void GameScene::hideAgents(std::vector<agentItem *> &agents)
{
    for (unsigned int i = 0; i < agents.size(); i++) {
        agentItem* current = agents.at(i);
        current->hide();
        // TODO: better toggling
        disconnect(current, &mapItem::mapItemMouseDragged, this, &GameScene::onMapItemMouseDragged);
        disconnect(current, &mapItem::mapItemMouseReleased, this, &GameScene::onMapItemMouseDropped);
        //current->setPos(300+current->boundingRect().width()*i,300);
    }
}

void GameScene::initPlayerHandFor(std::shared_ptr<Interface::Player> player)
{
    playerHands_.insert(std::make_pair(player, new PlayerHand(this, player)));
}

void GameScene::resourceInfo(ResourceMap &rmap, ResourceMap &dmap)
{
    resMap_ = rmap;
    demandsMap_ = dmap;
}

std::map<std::shared_ptr<const Interface::Player>, PlayerHand *> GameScene::playerHands()
{
    return playerHands_;
}

std::vector<LocationItem *> GameScene::GetLocItems()
{
    std::vector<LocationItem*> locItems = {};
    QList<QObject*> sceneChildren = children();
    for (auto child : sceneChildren) {
        LocationItem* loc = dynamic_cast<LocationItem*>(child);
        if (loc) {
            locItems.push_back(loc);
        }
    }
    return locItems;

}

ResourceMap GameScene::getResMap()
{
    return resMap_;
}

void GameScene::onPlayerChanged(std::shared_ptr<const Interface::Player> actingPlayer)
{
    // Updating gamescene who's currently playing
    std::shared_ptr<Interface::Game> gameboard = game_.lock();
    if (gameboard) {
        playerInTurn_ = gameboard->currentPlayer();
    }

    if (actingPlayer != game_.lock()->currentPlayer())
    {
        // If the player has been changed (round changed) then modify hand areas a bit

        // TODO: Coords for the hand that is going to be hidden are hard coded for now.
        // They should be based on the number of players in the game.
        // TODO: Action cards need to be set "face down" for the passing player
        // TPDP_ acotpm cards meed tp ne set "face up" for the player who is playing
        playerHands_.at(actingPlayer)->rearrange();
        playerHands_.at(actingPlayer)->setY(100);
        playerHands_.at(actingPlayer)->setScale(0.25);
        playerHands_.at(actingPlayer)->setEnabled(false);

        playerHands_.at(game_.lock()->currentPlayer())->setEnabled(true);
        playerHands_.at(game_.lock()->currentPlayer())->setY(600);
        playerHands_.at(game_.lock()->currentPlayer())->setScale(1);
        playerHands_.at(game_.lock()->currentPlayer())->show();

    } else {
        // The current player most likely got a new card in their hand, so rearrange the hand.
        qDebug() << "Player changed, turn was not changed";
       playerHands_.at(actingPlayer)->rearrange();
    }
}

void GameScene::initHands(std::shared_ptr<const Interface::Player> player)
{
    PlayerHand* hand = new PlayerHand(this, player);
    this->addItem(hand);
    playerHands_.insert(std::pair<std::shared_ptr<const Interface::Player>,PlayerHand*>(player, hand));
    hand->setPos(600, 400);
}

void GameScene::turnInfo(std::shared_ptr<Interface::Player> &currentplayer)
{
    playerInTurn_ = currentplayer;
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
    PopupDialog* clickDialog = new PopupDialog(locItem, playerInTurn_);
    clickDialog->show();

}

void GameScene::onActionDeclared(std::shared_ptr<Interface::ActionInterface> action)
{

    // TODO: check if the game is active
    if (!game_.lock())
    {
        qDebug() << "Action was declared on scene but there is no game";
        return;
    }
    emit actionDeclared(action);
    //playerHands_.at(game_.lock()->currentPlayer())->rearrange();
}
