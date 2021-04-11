#include "gamescene.hh"
#include "gamewindow.hh"
#include "agentitem.hh"
#include "mapitem.hh"
#include "locationitem.hh"
#include <cmath>
#include "carditem.hh"

GameScene::GameScene(QWidget *parent) : QGraphicsScene(parent)
{
    handAnchorCoords_ = std::make_pair(0, 400);
    handCardPadding_ = 5;
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        //QGraphicsItem* itemClicked = itemAt(event->scenePos(),QTransform());

        // Tää debugauskohta vaikeutti korttien liikuttelua joten kommentoin sen vaan ulos
        /*
        LocationItem* locItem = qgraphicsitem_cast<LocationItem*>(itemClicked);
        if (locItem and locItem->typeOf() == "locationitem") {

            locItem->mousePressEvent(event);
            selectedLocation = locItem;
            std::shared_ptr<Interface::AgentInterface> pointerAgent = nullptr;
            agentItem* age = new agentItem(pointerAgent);
            qDebug() << locItem->getObject()->name();
        } else {
            qDebug() << "tyhjä";
            selectedLocation = nullptr;
        }*/
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
        // mapScene->addItem(locationRect);
        drawItem(locationRect);
    }
}

void GameScene::drawItem(mapItem *item)
{
    addItem(item);
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

    // Show cards
    showHandCards();
}

void GameScene::showHandCards()
{
    float widthtotal = 0.0;
    int xStart;
    float widthPerCard;

    int count = handCards_.size();
    if (count) {
        for (int i = 0; i < count; ++i) {
            handCards_.at(i)->show();
            widthtotal += handCards_.at(i)->boundingRect().width();
        }
        widthPerCard = (widthtotal + handCardPadding_*count) / count;
        xStart =  handAnchorCoords_.first - (widthtotal / 2);

        for (int i = 0; i < count; ++i) {
            int x = (xStart + widthPerCard*i);
            handCards_.at(i)->setPos(x, handAnchorCoords_.second);
            qDebug() << handCards_.at(i)->x();
        }
    }
}

