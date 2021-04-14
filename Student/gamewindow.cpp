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
    gameui->graphicsView->setFixedSize(1000,800);
    // mapScene->setSceneRect(-600,600,-350,350);
    this->setFixedSize(1440, 900);
    this->setWindowTitle("SUSIPALATSI: TEH GAME");

    gameboard = std::make_shared<Interface::Game>();
    courseRunner = std::make_shared<Interface::Runner>(gameboard);

    mappi = PLAYER_STARTING_RESOURCES;

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

    setupPlayerStash();
    for (int i = 0 ; i < 3; i++) {
        spawnAgent(player1);
    }  
    for (int i = 0; i < 5; i++) {
        spawnAgent(player2);
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

    playerInTurn = player1;
    displayPlayerStats();
    initAreaResources();
    mapScene->resourceInfo(areaResourceMap);
    for (auto pair : areaResourceMap) {
        qDebug() << pair.first->name() << ": " << RESOURCE_NAMES.at(pair.second);
    }

    for (auto agent : playerAgents_.at(player1)) {
        qDebug() << agent->name();
    }

}

GameWindow::~GameWindow()
{
    delete gameui;
}

void GameWindow::drawLocations()
{

    std::vector<std::shared_ptr<Interface::Location>> locvec = getLocations();
    std::shared_ptr<Interface::Location> currentLocation = nullptr;
    mapScene->drawLocations(locvec);
}

void GameWindow::drawPlayerAgents(std::shared_ptr<Interface::Player> &player)
 {
    std::vector<std::shared_ptr<Interface::Agent>> agents = playerAgents_.at(player);
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
            CardItem *carditem = new CardItem(cards.at(i), this);
            // adds card to the scene
            mapScene->addItem(carditem);
        }
    }
}

void GameWindow::sendAgentTo(const std::shared_ptr<Interface::Location> &loc, std::shared_ptr<Interface::Player> &player)
{
    // Vanha funktio, jota ei käytetty. Todnäk turha jos drag&drop toimii jotain
    /*
    std::vector<agentItem*> listofAgents = playerAgents_.at(player);
    std::shared_ptr<Interface::AgentInterface> freeAgent = listofAgents.back()->getObject();
    qDebug() << "agent created";
    loc->sendAgent(freeAgent);
    qDebug() << "sent ;)";
    */

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

    playerAgents_.at(player).push_back(agentptr);
}

std::shared_ptr<Interface::Player> GameWindow::getPlayerInTurn()
{
    return playerInTurn;
}

void GameWindow::changeTurn()
{
    if (current_round % 2 == 0) {
        playerInTurn = player1;
    } else {
        playerInTurn = player2;
    }

    displayPlayerStats();

    mapScene->turnInfo(current_round, playerInTurn);

}

void GameWindow::listAgents(std::shared_ptr<Interface::Player> player)
{
    gameui->agentListWidget->clear();
    std::vector<std::shared_ptr<Interface::Agent>> listOfAgents = playerAgents_.at(player);
    for (auto agent : listOfAgents) {
        gameui->agentListWidget->addItem(agent->name());
    }
}

void GameWindow::setupPlayerStash()
{
    for (auto player : gameboard->players()) {

        // Map for agents each player has to use (empty on initialization)
        playerAgents_.insert({player, {}});

        // Map for players and their in-game currenty (2 coins on initialization)
        playerWallets_.insert({player, 2});

        // Map for players and their collected councilor cards (empty on initialization)
        councilorCards_.insert({player, {}});
    }
}

void GameWindow::displayPlayerStats()
{
    current_round++;
    gameui->currentRoundLabel->setText("Current round: " + QString::number(current_round));
    gameui->playerNameLabel->setText(playerInTurn->name());
    gameui->playerCoinsLabel->setText(QString::number(playerWallets_.at(playerInTurn)));
    gameui->councillorNumberLabel->setText(QString::number(councilorCards_.at(playerInTurn).size()) + " / 6");
    listAgents(playerInTurn);
}

void GameWindow::initAreaResources()
{
    int i = 1;
    for (auto loc : gameboard->locations()) {
        std::pair<std::shared_ptr<Interface::Location>, CommonResource> areaResourcePair;
        areaResourcePair = {loc, CommonResource(i)};
        areaResourceMap.insert(areaResourcePair);
        i++;
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


void GameWindow::on_passButton_clicked()
{
    changeTurn();
}
