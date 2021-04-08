#include "gamewindow.hh"
#include "ui_gamewindow.h"
#include "mapitem.hh"
#include "agentinterface.h"
#include "agentitem.hh"
#include "agentcard.hh"
#include "gamescene.hh"
#include <cmath>

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    gameui(new Ui::GameWindow)
{
    gameui->setupUi(this);
    // mapScene = new QGraphicsScene(gameui->graphicsView);
    mapScene = new GameScene(gameui->graphicsView);
    gameui->graphicsView->setScene(mapScene);

    // Asetetaan graphicViewin ja ikkunan koot staattiseks ensalkuun
    gameui->graphicsView->setFixedSize(1200, 700);
    this->setFixedSize(1400, 900);

    this->setWindowTitle("SUSIPALATSI: TEH GAME");

    courseGameScene = std::make_shared<Interface::Game>();
    courseRunner = std::make_shared<Interface::Runner>(courseGameScene);

    // Luodaan location-oliot
    for (int i = 0; i < 6; i++) {
        std::shared_ptr<Interface::Location> location = std::make_shared<Interface::Location>(i, paikat_.at(i));
        courseGameScene->addLocation(location);
    }
    drawLocations();
}

GameWindow::~GameWindow()
{
    delete gameui;
}

void GameWindow::drawLocations()
{
    int buildingCtr = 0;
    std::vector<std::shared_ptr<Interface::Location>> locvec = getLocations();
    std::shared_ptr<Interface::Location> currentLocation = nullptr;


    // Piirretään rakennukset "ympyrän" kehälle
    const int xCenter = 0;
    const int yCenter = 0;
    const int radius = 300;

    int locationCount = courseGameScene->locations().size();
    const int degree = 360 / locationCount;

    for (int i = 0; i < locationCount; i++) {
        currentLocation = locvec.at(buildingCtr);
        mapItem* locationRect = new mapItem(currentLocation);
        mapScene->addItem(locationRect);

        // Geometrinen sijainti kehällä
        int angleDeg = degree * i;
        float angleRad = angleDeg * M_PI / 180;
        int x = xCenter + radius * std::cos(angleRad);
        int y = yCenter + radius * std::sin(angleRad);
        locationRect->setCoords(x, y);

        buildingCtr++;
    }


}

const std::vector<std::shared_ptr<Interface::Location> > GameWindow::getLocations()
{
    return courseGameScene->locations();
}
/*
void GameWindow::drawAgents(mapItem *&drawLocation)
{
    std::shared_ptr<Interface::Location> loc = drawLocation->getObject();
    std::set<std::shared_ptr<Interface::AgentInterface>> locAgents = loc->agents();
    int counter = 0;
    std::pair<int, int> locationXY = drawLocation->getCoords();
    const int xCenter = locationXY.first;
    const int yCenter = locationXY.second;
    const int radius = 50;

    int agentCount = locAgents.size();
    const int degree = 360 / agentCount;

    for (auto agent : locAgents) {
        std::shared_ptr<agentCard> paikkaAgentti = std::make_shared<agentCard>(agent);
        mapItem* agentRect = new agentItem(paikkaAgentti);
        mapScene->addItem(agentRect);

        int angleDeg = degree * counter;
        float angleRad = angleDeg * M_PI / 180;
        int x = xCenter + radius * std::cos(angleRad);
        int y = yCenter + radius * std::sin(angleRad);
        agentRect->setCoords(x, y);
        counter++;
    }
}
*/
