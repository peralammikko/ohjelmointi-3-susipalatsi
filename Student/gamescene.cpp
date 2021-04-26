#include "gamescene.hh"
#include "gamewindow.hh"
#include "agentitem.hh"
#include "mapitem.hh"
#include "locationitem.hh"

#include "carditem.hh"
#include "popupdialog.hh"
#include "agentdialog.hh"
#include "game.h"

#include "player.h"



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

GameScene::~GameScene()
{
    delete arrow1_;
    delete arrow2_;

}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "mouse pos on click:" <<event->scenePos();
    update();
    QGraphicsScene::mousePressEvent(event);
}

void GameScene::drawLocations(std::vector<std::pair<std::shared_ptr<Interface::Location>, std::vector<std::pair<QString, QString>>>> locationInformation)
{
    std::shared_ptr<Interface::Location> currentLocation = nullptr;
    int locationCount = locationInformation.size();

    for (int i = 0; i < locationCount; i++) {
        currentLocation = locationInformation.at(i).first;
        std::shared_ptr<Interface::CommonResource> localRes = resMap_.at(currentLocation);
        std::shared_ptr<Interface::CommonResource> demandRes = demandsMap_.at(currentLocation);

        LocationItem* locItem = new LocationItem(currentLocation, locationInformation.at(i).second, localRes, demandRes);
        connect(locItem, &LocationItem::locationItemPressed, this, &GameScene::onLocationItemClicked);
        // locItem->setLocalResource(localRes);
        // locItem->setDemandedResource(demandRes);

        drawItem(locItem);
        locItem->setParent(this);
        locationItems_.push_back(locItem);
    }
    shuffleLocationItems();
    rearrangeLocationItems();
    resetLocationNeighbours();
}

void GameScene::drawItem(mapItem *item)
{
    addItem(item);
}

void GameScene::initPlayerHandFor(std::shared_ptr<Interface::Player> player)
{
    playerHands_.insert(std::make_pair(player, new PlayerHand(player)));
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
    declaringMapItem->setHome(declaringMapItem->parentItem()->mapFromScene(solarsystemCenter_));
    declaringMapItem->goHome();
    declaringMapItem->setWaitingForAction(true);
}

void GameScene::resetAction()
{
    if (declaringMapItem_ != nullptr and declaringMapItem_->parentItem() != nullptr){
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
        auto previousHand = playerHands_.at(actingPlayer);
        auto currentHand = playerHands_.at(game_.lock()->currentPlayer());

        previousHand->rearrange();
        previousHand->setHome(QPointF(SPACEBETWEENHANDS*actingPlayer->id(), 0) + waitingPlayerHandAnchor_);
        previousHand->goHome();
        previousHand->setScale(0.25);
        previousHand->setEnabled(false);

        currentHand->rearrange();
        currentHand->setEnabled(true);
        currentHand->setHome(activePlayerHandAnchor_);
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
                        aItem->setScale(0.5);
                    } else if (aItem->getAgentClass()->owner().lock() == game_.lock()->currentPlayer())
                    {
                        aItem->setEnabled(true);
                        aItem->setScale(1);
                    }
                }
            }
        }

        currentHand->show();

    } else {
        // The current player most likely got a new card in their hand, so rearrange the hand.
        qDebug() << "Player changed, turn was not changed";
       playerHands_.at(actingPlayer)->rearrange();
    }
}

void GameScene::initHands(std::shared_ptr<const Interface::Player> player)
{
    PlayerHand* hand = new PlayerHand(player);
    this->addItem(hand);
    playerHands_.insert(std::pair<std::shared_ptr<const Interface::Player>,PlayerHand*>(player, hand));
    hand->setPos(600, 400);
}

void GameScene::addActionCardForPlayer(std::shared_ptr<const Interface::Player> player, std::shared_ptr<Interface::ActionCard> card)
{
    if (card->owner().lock() == player){
        CardItem* cardItem = new CardItem(card, this);
        connect(cardItem, &CardItem::actionDeclared, this, &GameScene::onActionDeclared);
        addItem(cardItem);
        playerHands_.at(player)->addMapItem(cardItem);
    }
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

void GameScene::onLocationItemClicked(LocationItem* locItem)
{
    // Close any dialog remaining on screen before creating a new one
    if (clickDialog) {
        clickDialog->close();
    }
    clickDialog = new PopupDialog(locItem, playerInTurn_);
    clickDialog->show();
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
}

void GameScene::rearrangeLocationItems()
{

    solarsystemCenter_ = QPointF(this->width()/2, this->height()*2/6);
    const int radius = 320;
    int locationCount = locationItems_.size();
    const int degree = 360 / locationCount;

    for (int i = 0; i < locationCount; i++) {
        auto currentLocItem = locationItems_.at(i);

        // Geometrinen sijainti kehällä
        float angleDeg = degree * i;
        float angleRad = angleDeg * M_PI / 180;

        float x = solarsystemCenter_.x() + radius * std::cos(angleRad);
        // Squash the y a little bit
        float y = solarsystemCenter_.y()  - 0.7* radius * std::sin(angleRad);
        currentLocItem->setHome(QPointF(x,y));
        currentLocItem->goHome();
    }
}

void GameScene::resetLocationNeighbours()
{
    std::pair<LocationItem*, LocationItem*> neighbours;
    unsigned int locCount = locationItems_.size();
    if (locCount > 2) {
        for (unsigned int i = 0; i < locCount; ++i)
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
                //declaringMapItem->~mapItem();
                delete declaringMapItem;
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

void GameScene::onEnteringNextRound()
{
    shuffleLocationItems();
    rearrangeLocationItems();
    resetLocationNeighbours();
}

