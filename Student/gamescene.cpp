#include "gamescene.hh"
#include "gamewindow.hh"
#include "agentitem.hh"
#include "mapitem.hh"
#include "locationitem.hh"
#include <cmath>
#include "carditem.hh"

// required for signaling??
#include <QObject>

GameScene::GameScene(QWidget *parent) : QGraphicsScene(parent)
{
    handAnchorCoords_ = std::make_pair(0, 400);
    handCardPadding_ = 5;
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {

        QGraphicsItem* itemClicked = itemAt(event->scenePos(), QTransform());
        LocationItem* locItem = qgraphicsitem_cast<LocationItem*>(itemClicked);
        agentItem* agItem = qgraphicsitem_cast<agentItem*>(itemClicked);
        if (locItem and locItem->typeOf() == "locationitem") {
            locItem->mousePressEvent(event);
            selectedLocation = locItem;
            qDebug() << locItem->getObject()->name();
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
    }
}

void GameScene::drawLocations(std::vector<std::shared_ptr<Interface::Location>> &locvec)
{
    std::shared_ptr<Interface::Location> currentLocation = nullptr;

    // Piirretään rakennukset "ympyrän" kehälle
    const int xCenter = this->width()/2;
    const int yCenter = this->height()/2;
    //qDebug() << "Center:" <<xCenter << yCenter;
    const int radius = 300;

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
    int agentsCount = agents.size();
    for (int i = 0; i < agentsCount; i++) {

        agentItem* current = agents.at(i);
        float x = 200+50*i;
        std::shared_ptr<Interface::AgentInterface> agent = current->getObject();

        current->setX(x);
        current->setY(400);
        //current->setPos(x,400);
        qDebug() <<x<<current->mapRectFromScene(current->boundingRect());
    }
}

void GameScene::createHandCards(std::vector<std::shared_ptr<Interface::CardInterface>> cards)
{
    for (unsigned int i = 0; i < cards.size(); ++i) {
        CardItem *carditem = new CardItem(cards.at(i));
        // adds card to the scene
        this->addItem(carditem);
        carditem->hide();
        handCards_.push_back(carditem);
    }
    showHandCards();
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

void GameScene::onCardDragged(CardItem* card)
{
    // Get every item under cardboundaries
    QList<QGraphicsItem*> items = card->collidingItems();

    int count =0;
    for (int i = 0; i < items.size(); ++i)
    {
        count +=1;
        if (items.at(i) != card) {
             // Followin allows us to get ANY type of interaface data under the rect
             LocationItem* location =dynamic_cast<LocationItem*>(items.at(i));
             if (location != nullptr)
             {
                 qDebug() << "Olen rakennus" << location->getObject()->name();
             }
        } else {
            qDebug() << "found myself" << items.at(i)->type();
        }
    }
}


void GameScene::onCardDropped(CardItem* card)
{
     qDebug() << "i am dropped" << card->x() << card->y()<<card->mapRectFromScene(card->boundingRect());
     card->setPos(card->pos());
}

