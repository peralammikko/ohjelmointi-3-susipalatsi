#include "gamescene.hh"
#include "gamewindow.hh"
#include "agentitem.hh"
#include "mapitem.hh"
#include "locationitem.hh"

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
    arrow1_ = new SceneArrow(nullptr, nullptr);
    addItem(arrow1_);
    arrow2_ = new SceneArrow(nullptr, nullptr);
    addItem(arrow2_);
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
    int locationCount = locvec.size();

    for (int i = 0; i < locationCount; i++) {
        currentLocation = locvec.at(i);
        LocationItem* locItem = new LocationItem(currentLocation, i);
        connect(locItem, &LocationItem::locationItemPressed, this, &GameScene::onLocationItemClicked);
        Interface::CommonResource localRes = resMap_.at(currentLocation);
        locItem->setLocalResource(localRes);
        Interface::CommonResource demandRes = demandsMap_.at(currentLocation);
        locItem->setDemandedResource(demandRes);
        drawItem(locItem);
        locItem->setParent(this);
        locationItems_.push_back(locItem);
    }
    shuffleLocationItems();
    rearrangeLocationItems();
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

void GameScene::prepareForAction(std::shared_ptr<Interface::ActionInterface> action, mapItem *declaringMapItem)
{
    declaredAction_ = action;
    declaringMapItem_ = declaringMapItem;

    // Set visual arrows
    arrow1_->setEndItem(declaringMapItem);
    arrow2_->setStartItem(declaringMapItem);
    auto homeItem = dynamic_cast<mapItem*>(declaringMapItem->parentItem());
    if (homeItem){
        arrow1_->setStartItem(homeItem);
        arrow1_->updatePosition();
    }
    auto agAc = std::dynamic_pointer_cast<AgentActionInterface>(action);
    if (agAc) {
        arrow2_->setEndItem(agAc.get()->getTargetMapItem());
        arrow2_->updatePosition();
    }
    declaringMapItem->setHome(declaringMapItem->parentItem()->mapFromScene(QPointF(600,350)));
    declaringMapItem->goHome();
    declaringMapItem->setWaitingForAction(true);
}

void GameScene::resetAction()
{
    auto parentMapItem = dynamic_cast<mapItem*>(declaringMapItem_->parentItem());
    if (parentMapItem){
        parentMapItem->rearrange();
    }

    declaredAction_ = std::shared_ptr<Interface::ActionInterface>();
    declaringMapItem_->setWaitingForAction(false);
    declaringMapItem_ = nullptr;

    arrow1_->setStartItem(nullptr);
    arrow1_->setEndItem(nullptr);
    arrow1_->hide();

    arrow2_->setStartItem(nullptr);
    arrow2_->setEndItem(nullptr);
    arrow2_->hide();

}

void GameScene::onPlayerChanged(std::shared_ptr<const Interface::Player> actingPlayer)
{
    if (actingPlayer != game_.lock()->currentPlayer())
    {
        // If the player has been changed (round changed) then modify hand areas a bit

        // TODO: Coords for the hand that is going to be hidden are hard coded for now.
        // They should be based on the number of players in the game.
        // TODO: Action cards need to be set "face down" for the passing player
        // TODO: Action cards need to be set "face up" for the player who is playing
        auto previousHand = playerHands_.at(actingPlayer);
        auto currentHand = playerHands_.at(game_.lock()->currentPlayer());

        previousHand->rearrange();
        previousHand->setHome(currentHand->pos());
        previousHand->goHome();
        previousHand->setScale(0.25);
        previousHand->setEnabled(false);

        currentHand->rearrange();
        currentHand->setEnabled(true);
        currentHand->setHome(previousHand->pos());
        currentHand->goHome();
        currentHand->setScale(1);

        for (unsigned int i = 0; i < locationItems_.size(); ++i){
            auto childItems = locationItems_.at(i)->childItems();
            for (int j = 0; j < childItems.size(); ++j){
                auto aItem = dynamic_cast<agentItem*>(childItems.at(j));
                if (aItem)
                {
                    if (aItem->getAgentClass()->owner().lock() == actingPlayer)
                    {
                        aItem->setEnabled(false);
                    } else if (aItem->getAgentClass()->owner().lock() == game_.lock()->currentPlayer())
                    {
                        aItem->setEnabled(true);
                    }
                }
            }
        }
        // TODO: the game somewhere hides the first player hand which is annoying
        currentHand->show();

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
    // This is unfortunately broken for a moment
    //auto l = game_.lock()->currentPlayer();
  //  PopupDialog* clickDialog = new PopupDialog(locItem, &l );
    //clickDialog->setParent(this);
    // clickDialog->show();
}

void GameScene::shuffleLocationItems()
{
    // for now we just shuffle EVERYTHING
    std::pair<LocationItem*, LocationItem*> neighbours;
    unsigned int locCount = locationItems_.size();
    for (unsigned int i = 1; i < locCount; ++i)
    {
        unsigned int j = Interface::Random::RANDOM.uint(i);
        if (i != j)
        {
            std::swap(locationItems_.at(i), locationItems_.at(j));
        }
    }
    if (locCount > 2) {
        for (unsigned int i = 1; i < locCount; ++i)
        {
            int j = i+1;
            int k = i-1;
            if (i == 0){
                k = locCount-1;
            } else if (i == locCount-1) {
                j = 0;
            }
            neighbours = {locationItems_.at(j), locationItems_.at(k)};
            locationItems_.at(i)->setNeighbours(neighbours);
        }
    } else {
        neighbours = {locationItems_.at(locCount-1), locationItems_.at(locCount-1)};
        locationItems_.at(0)->setNeighbours(neighbours);
        neighbours = {locationItems_.at(0), locationItems_.at(0)};
        locationItems_.at(locCount-1)->setNeighbours(neighbours);
    }
}

void GameScene::rearrangeLocationItems()
{
    // Piirretään rakennukset "ympyrän" kehälle
    const int xCenter = this->width()/2;
    const int yCenter = this->height()*2/5;

    const int radius = 350;
    int locationCount = locationItems_.size();
    const int degree = 360 / locationCount;

    for (int i = 0; i < locationCount; i++) {
        auto currentLocItem = locationItems_.at(i);

        // Geometrinen sijainti kehällä
        float angleDeg = degree * i;
        float angleRad = angleDeg * M_PI / 180;

        float x = xCenter + radius * std::cos(angleRad);
        // Squash the y a little bit
        float y = yCenter  - 0.7* radius * std::sin(angleRad);
        currentLocItem->setHome(QPointF(x,y));
        currentLocItem->goHome();
    }
}

void GameScene::onActionDeclared(std::shared_ptr<Interface::ActionInterface> action, mapItem *declaringMapItem, bool resetting)
{
    if (game_.lock() and game_.lock()->active())
    {
        if (resetting){
            qDebug() << "Resetting action";
            resetAction();
            return;
        }
        if (declaredAction_.get() and declaringMapItem_){
            if (declaringMapItem->typeOf()=="actioncard"){
                // Remove card from its owner's hand and put it in its home location's discard pile
                auto card = dynamic_cast<CardItem*>(declaringMapItem);
                auto cardOwner = card->getCard()->owner().lock();
                if (cardOwner) {
                    cardOwner->playCard(card->getCard());
                    auto location = card->getCard()->location().lock();
                    if (location) {
                        location->discards().get()->addCard(card->getCard());
                    }
                }
                declaringMapItem->~mapItem();
                playerHands_.at(game_.lock()->currentPlayer())->rearrange();
                // TODO: rearrange building agents too
                emit actionDeclared(declaredAction_);
                resetAction();
            } else {
                declaringMapItem->goHome();
                resetAction();
                return;

            }
        } else {
            prepareForAction(action, declaringMapItem);
        }
    } else {
        qDebug() << "Action was declared on scene but there is no game";
        return;
    }
}

