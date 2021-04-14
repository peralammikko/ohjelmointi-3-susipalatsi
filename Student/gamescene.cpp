#include "gamescene.hh"
#include "gamewindow.hh"
#include "agentitem.hh"
#include "mapitem.hh"
#include "locationitem.hh"
#include <cmath>
#include "carditem.hh"
#include "popupdialog.hh"


// required for signaling??
#include <QObject>

GameScene::GameScene(QWidget *parent) : QGraphicsScene(parent)
{
    handAnchorCoords_ = std::make_pair(0, 400);
    handCardPadding_ = 5;
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    /*
    if (event->button() == Qt::LeftButton) {

        QGraphicsItem* itemClicked = itemAt(event->scenePos(), QTransform());
        LocationItem* locItem = qgraphicsitem_cast<LocationItem*>(itemClicked);
        agentItem* agItem = qgraphicsitem_cast<agentItem*>(itemClicked);
        if (locItem and locItem->typeOf() == "locationitem") {
            CommonResource res = resMap_.at(locItem->getObject());
            int BV = locItem->getBasevalue();
            PopupDialog* clickDialog = new PopupDialog(locItem->getObject(), BV, res, playerInTurn_);
            clickDialog->show();

        } else if (agItem and agItem->typeOf() == "agentitem") {
            selectedAgent = agItem;
            // Gamescenen ei tarvitse erikseen huolehtia mousepress eventtejä.
            // Kaikki mousepressit voidaan käistellä luokkakohtaisesti, joten tavoitteena käyttää niit siellä
            // Toistaiseksi kommentoin seuraavan alta pois
            // agItem->mousePressEvent(event);
            agItem->testPrint();
        } else {
            selectedAgent = nullptr;
            selectedLocation = nullptr;
        }
    }*/
    update();
    QGraphicsScene::mousePressEvent(event);
}

void GameScene::drawLocations(std::vector<std::shared_ptr<Interface::Location>> &locvec)
{
    std::shared_ptr<Interface::Location> currentLocation = nullptr;

    // Piirretään rakennukset "ympyrän" kehälle
    const int xCenter = this->width()/2;
    const int yCenter = this->height()/2;
    const int radius = 250;

    // Needs more elegant implementation, like a global constant and a scaling value
    const int radius = 300;
    // by default we have 6 locations
    int locationCount = locvec.size();
    const int degree = 360 / locationCount;

    for (int i = 0; i < locationCount; i++) {
        currentLocation = locvec.at(i);
        LocationItem* locationRect = new LocationItem(currentLocation);

        // Geometrinen sijainti kehällä
        int angleDeg = degree * i;
        float angleRad = angleDeg * M_PI / 180;
        int x = xCenter + radius * std::cos(angleRad);
        int y = yCenter + radius * std::sin(angleRad);
        locationRect->setCoords(x, y);
        drawItem(locationRect);
    }
}

void GameScene::drawItem(mapItem *item)
{
    addItem(item);
}

void GameScene::drawAgents(std::vector<agentItem *> &agents)
{
    for (unsigned int i = 0; i < agents.size(); i++) {
        agentItem* current = agents.at(i);
        //std::shared_ptr<Interface::AgentInterface> agent = current->getObject(); // What is this for ?
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

void GameScene::resourceInfo(AreaResources &rmap)
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
                 qDebug() << "Olen rakennus" << location->getObject()->name();
             } else {
                agentItem* agent = dynamic_cast<agentItem*>(items.at(i));
                if (agent != nullptr)
                {
                    qDebug() << "Olen agentti" << agent->getObject()->name();
                }
             }
        }
    }
}

void GameScene::onMapItemMouseDropped(mapItem* mapitem)
{
    // TODO: implement logic to see if the move is legal
     qDebug() << "a mapitem has been dropped";
     mapitem->goHome();
     // Emit signal which is tied to game logic.
     // Game logic then calls a public method in gamescene which
     // makes the card item either go back if illegal or do a legal action.
}

