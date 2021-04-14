#include "gamewindow.hh"
#include "ui_gamewindow.h"
#include "mapitem.hh"
#include "locationitem.hh"
#include "agent.hh"
#include "agentitem.hh"
#include "gamescene.hh"
#include <cmath>

// kokeilu käden asettelulle
#include <QGraphicsLinearLayout>
#include <QGraphicsWidget>


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

    gameui->graphicsView->setMouseTracking(true);

    // Asetetaan graphicViewin ja ikkunan koot staattiseks ensalkuun
    gameui->graphicsView->setFixedSize(1400, 900);
   // mapScene->setSceneRect(-600,600,-350,350);
    this->setFixedSize(1920, 1080);
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

    gameboard->addPlayer(player1);
    gameboard->addPlayer(player2);
    std::pair<std::shared_ptr<Interface::Player>, std::vector<agentItem*>> pair1(player1, {});
    std::pair<std::shared_ptr<Interface::Player>, std::vector<agentItem*>> pair2(player2, {});

    playerAgents_.insert(pair1);
    playerAgents_.insert(pair2);

    playerInTurn = player1;
    drawPlayerAgents(playerInTurn);
    getPlayerInTurn();

    for (int i = 0 ; i < 3; i++) {
        spawnAgent(player1);
    }
    
    for (int i = 0; i < 5; i++) {
        spawnAgent(player2);
    }

    if (current_round == 0) {
        drawPlayerAgents(player1);
    } else {
        drawPlayerAgents(player2);
    }

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

void GameWindow::drawPlayerAgents(std::shared_ptr<Interface::Player> &player)
 {
    std::vector<agentItem*> agents = playerAgents_.at(player);
    mapScene->drawAgents(agents);
 }

void GameWindow::drawItem(mapItem *item)
{
    mapScene->drawItem(item);
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

void GameWindow::sendAgentTo(const std::shared_ptr<Interface::Location> &loc, std::shared_ptr<Interface::Player> &player)
{
    std::vector<agentItem*> listofAgents = playerAgents_.at(player);
    std::shared_ptr<Interface::AgentInterface> freeAgent = listofAgents.back()->getObject();
    qDebug() << "agent created";
    loc->sendAgent(freeAgent);
    qDebug() << "sent ;)";

}

void GameWindow::spawnAgent(std::shared_ptr<Interface::Player> &player)
{
    // Create agent interface, which holds all of the data of the card.
    // Ideally we want this to be handled by carddata class, which would use xml/JSON later on
    // For now we will just use some default generated stuff
    QString agname{"Perry"};
    std::shared_ptr<Interface::Agent> agentptr = std::make_shared<Interface::Agent>(agname + player->name(), "Agent");

    agentItem* agenttiesine = new agentItem(agentptr);
    mapScene->addItem(agenttiesine);

    playerAgents_.at(player).push_back(agenttiesine);
}

std::shared_ptr<Interface::Player> GameWindow::getPlayerInTurn()
{
    return playerInTurn;
}
