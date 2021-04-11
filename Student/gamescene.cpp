#include "gamescene.hh"
#include "gamewindow.hh"
#include "agentitem.hh"
#include "mapitem.hh"
#include "locationitem.hh"
#include <cmath>
#include "playerclass.hh"

GameScene::GameScene(QWidget *parent) : QGraphicsScene(parent)
{

}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QGraphicsItem* itemClicked = itemAt(event->scenePos(),QTransform());
        LocationItem* locItem = qgraphicsitem_cast<LocationItem*>(itemClicked);
        if (locItem and locItem->typeOf() == "locationitem") {
            locItem->mousePressEvent(event);
            selectedLocation = locItem;
            qDebug() << locItem->getObject()->name();
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
    const int xCenter = 0;
    const int yCenter = 0;
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
        current->setCoords(400+i*80, 400);
        drawItem(current);
    }
}
