#include "gamewindow.hh"
#include "ui_gamewindow.h"
#include "mapitem.hh"
#include "locationitem.hh"
#include "agentinterface.h"
#include "agentitem.hh"
#include "gamescene.hh"
#include <cmath>

#include "actioncard.hh"

#include "playerhand.hh"


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

    gameboard = std::make_shared<Interface::Game>();
    courseRunner = std::make_shared<Interface::Runner>(gameboard);

    // Luodaan location-oliot
    for (int i = 0; i < 6; i++) {
        std::shared_ptr<Interface::Location> location = std::make_shared<Interface::Location>(i, paikat_.at(i));
        gameboard->addLocation(location);
    }
    drawLocations();

    // luodaan pari pelaajaa
    for (int i=0; i<2; i++) {
        gameboard->addPlayer(QString::number(i));
    }
    // luodaan pelaajille käsialueen luokka
    for (unsigned int i=0; i<gameboard->players().size(); ++i) {
        std::shared_ptr<Interface::Player> pl = gameboard->players().at(i);
        std::shared_ptr<PlayerHand> hand = std::make_shared<PlayerHand>(mapScene, pl, 0, -100 + 200*i);
        hands_.insert(make_pair(pl, hand));

        // Luodaan pari korttia ja annetaan ne pelaajalle
        for (int j=0; j<4; ++j) {
            std::shared_ptr<Interface::ActionCard> card = std::make_shared<Interface::ActionCard>();

         //   card = std::shared_ptr<Interface::CardInterface>();
            pl->addCard(card);
            //std::make_shared<Interface::CardInterface>();
            //std::shared_ptr<Interface::CardInterface> card = std::make_shared<Interface::CardInterface>();
            
        }
    }
}

GameWindow::~GameWindow()
{
    delete gameui;
}

void GameWindow::drawLocations()
{
    //
    std::vector<std::shared_ptr<Interface::Location>> locvec = getLocations();
    std::shared_ptr<Interface::Location> currentLocation = nullptr;
    mapScene->drawLocations(locvec);
}

void GameWindow::drawItem(mapItem *item)
{
    mapScene->drawItem(item);
}

const std::vector<std::shared_ptr<Interface::Location> > GameWindow::getLocations()
{
    return gameboard->locations();
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
        agentItem* agenttismies = new agentItem(agent);
        mapScene->addItem(agenttismies);

        int angleDeg = degree * counter;
        float angleRad = angleDeg * M_PI / 180;
        int x = xCenter + radius * std::cos(angleRad);
        int y = yCenter + radius * std::sin(angleRad);
        agenttismies->setCoords(x, y);
        counter++;
    }
}
*/

