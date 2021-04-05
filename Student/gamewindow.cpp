#include "gamewindow.hh"
#include "ui_gamewindow.h"
#include "mapitem.hh"

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    gameui(new Ui::GameWindow)
{
    gameui->setupUi(this);
    gameScene = new QGraphicsScene(gameui->graphicsView);
    gameui->graphicsView->setScene(gameScene);

    this->setWindowTitle("SUSIPALATSI: TEH GAME");

    courseGameScene = std::make_shared<Interface::Game>();

    for (int i = 0; i < 6; i++) {
        std::shared_ptr<Interface::Location> location = std::make_shared<Interface::Location>(i, paikat_.at(i));
        courseGameScene->addLocation(location);
    }
    qDebug() << "locations added";
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

    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < 3; j++) {
            currentLocation = locvec.at(buildingCtr);
            mapItem* locationRect = new mapItem(currentLocation);
            if (j==1) {
                locationRect->setCoords(-150,j*250);
            } else {
                locationRect->setCoords(i*250, j*250);
            }
            gameScene->addItem(locationRect);
            buildingCtr++;
        }
    }

    for (int i = 1; i < 2; i++) {
        for (int j = 2; j >= 0; j--) {
            currentLocation = locvec.at(buildingCtr);
            mapItem* locationRect = new mapItem(currentLocation);
            if (j == 1) {
                locationRect->setCoords(400,j*250);
            } else {
                locationRect->setCoords(250, j*250);
            }
            gameScene->addItem(locationRect);
            buildingCtr++;
        }
    }
}
