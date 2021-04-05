#include "gamewindow.hh"
#include "ui_gamewindow.h"
#include "mapitem.hh"
#include <cmath>

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    gameui(new Ui::GameWindow)
{
    gameui->setupUi(this);
    mapScene = new QGraphicsScene(gameui->graphicsView);
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
    std::vector<std::shared_ptr<Interface::Location>> locvec = courseGameScene->locations();
    std::shared_ptr<Interface::Location> currentLocation = nullptr;


    // Piirretään rakennukset "ympyrän" kehälle
    const int xCenter = 0;
    const int yCenter = 0;
    const int radius = 200;

    int locationCount = courseGameScene->locations().size();
    const int degree = 360 / locationCount;

    for (int i = 0; i < locationCount; i++) {
        currentLocation = locvec.at(buildingCtr);
        mapItem* locationRect = new mapItem(currentLocation);
        gameScene->addItem(locationRect);

        // Geometrinen sijainti kehällä
        int angleDeg = degree * i;
        float angleRad = angleDeg * M_PI / 180;
        int x = xCenter + radius * std::cos(angleRad);
        int y = yCenter + radius * std::sin(angleRad);
        locationRect->setCoords(x, y);

        buildingCtr++;
    }


}
