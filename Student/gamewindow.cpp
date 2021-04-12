#include "gamewindow.hh"
#include "ui_gamewindow.h"
#include "mapitem.hh"
#include "locationitem.hh"
#include "agentinterface.h"
#include "agentitem.hh"
#include "gamescene.hh"
#include <cmath>

// kokeilu käden asettelulle
#include <QGraphicsLinearLayout>
#include <QGraphicsWidget>


#include "actioncard.hh"

#include "playerhand.hh"
#include "playerclass.hh"


GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    gameui(new Ui::GameWindow)
{
    gameui->setupUi(this);
    // mapScene = new QGraphicsScene(gameui->graphicsView);
    mapScene = new GameScene(gameui->graphicsView);
    gameui->graphicsView->setScene(mapScene);

    gameui->graphicsView->setMouseTracking(true);

    // Asetetaan graphicViewin ja ikkunan koot staattiseks ensalkuun
    gameui->graphicsView->setFixedSize(1400, 900);
   // mapScene->setSceneRect(-600,600,-350,350);
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

    player1 = std::make_shared<Interface::Player>(gameboard, 1, "RED");
    player2 = std::make_shared<Interface::Player>(gameboard, 2, "BLUE");

    /*
    std::shared_ptr<Playerclass> p1 = std::make_shared<Playerclass>(player1);
    std::shared_ptr<Playerclass> p2 = std::make_shared<Playerclass>(player2);
    */
    Playerclass p1(player1);
    Playerclass p2(player2);

    for (int i = 0 ; i < 3; i++) {
        p1.spawnAgent();
    }
    drawPlayerAgents(p1);

    // luodaan pari pelaajaa
    for (int i=0; i<2; i++) {
        gameboard->addPlayer(QString::number(i));
    }
    // luodaan pelaajille käsialueen luokka
    for (unsigned int i=0; i<gameboard->players().size(); ++i) {
        std::shared_ptr<Interface::Player> pl = gameboard->players().at(i);

        // Luodaan pari korttia ja annetaan ne pelaajalle
        for (int j=0; j<4; ++j) {
            std::shared_ptr<Interface::ActionCard> card = std::make_shared<Interface::ActionCard>();
            pl->addCard(card);


        }
        // Tehdäänkin cardItemit vain silloin kun niitä tarvitaan. saatta aiheuttaa lagia mut ihan sama
        /*
            CardItem *carditem = new CardItem(card);

            // adds card to the scene
            //mapScene->addItem(carditem);

            // adds card to layout
            hand->addItem(carditem);
        }


        // Layout needs a container. Cards are placed in a layout within a container.
        QGraphicsWidget* container = new QGraphicsWidget;
        container->setX(200*i);

        container->setLayout(hand);
        container->update();
        mapScene->addItem(container);


        //hand->setGeometry(0, i*500, 100, 100);

        //does not work
        //container->setGeometry(0, i*500, 100, 100);



        //qDebug() << hand->geometry() << container->layout();
        playerhands_.insert(make_pair(pl, container));
        //container->setPos();
        //container->setPos(std::make_pair(0, -100 + 200* i));
        //container->hide();
        */
    }
    mapScene->createHandCards(gameboard->players().at(0)->cards());

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

void GameWindow::drawPlayerAgents(Playerclass &p)
{
    std::vector<agentItem*> agents = p.getAgents();
    mapScene->drawAgents(agents);
}

void GameWindow::drawItem(mapItem *item)
{
    mapScene->drawItem(item);
}

void GameWindow::sendAgentTo(LocationItem *loc)
{
    return;
}

const std::vector<std::shared_ptr<Interface::Location> > GameWindow::getLocations()
{
    return gameboard->locations();
}

void GameWindow::enablePlayerHand(std::shared_ptr<Interface::Player> player)
{
    std::vector<std::shared_ptr<Interface::Player>> players = gameboard->players();
    // Player must exist in game class
    if (player and std::find(players.begin(), players.end(), player) != players.end())
    {
        std::vector<std::shared_ptr<Interface::CardInterface>> cards = player->cards();
        std::vector<CardItem> carditems;
        for (unsigned int i = 0; i < cards.size(); ++i)
        {
            CardItem *carditem = new CardItem(cards.at(i));
            // adds card to the scene
            mapScene->addItem(carditem);

        }

    }
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

