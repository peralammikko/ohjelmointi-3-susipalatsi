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
#include "../Course/manualcontrol.h"
#include "commonresource.hh"


GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    gameui_(new Ui::GameWindow)
{
    gameui_->setupUi(this);

    // Declare the game first before gameScene, so we can give game_ to gameScene's constructor
    game_ = std::make_shared<Interface::Game>();
    //  courseRunner = std::make_shared<GameRunner>(game_);

    gameScene_ = new GameScene(gameui_->graphicsView, game_);
    gameui_->graphicsView->setScene(gameScene_);

    gameui_->graphicsView->setMouseTracking(true);

    // Asetetaan graphicViewin ja ikkunan koot staattiseks ensalkuun
    gameui_->graphicsView->setFixedSize(1400, 900);
    // gameScene_->setSceneRect(-600,600,-350,350);
    this->setFixedSize(1920, 1080);
    this->setWindowTitle("SUSIPALATSI: TEH GAME");

    // Luodaan location-oliot
    for (int i = 0; i < 6; i++) {
        std::shared_ptr<Interface::Location> location = std::make_shared<Interface::Location>(i, paikat_.at(i));
        game_->addLocation(location);
    }
    initAreaResources();
    gameScene_->resourceInfo(initResourceMap_);

    drawLocations();

    player1 = std::make_shared<Interface::Player>(game_, 1, "RED");
    player2 = std::make_shared<Interface::Player>(game_, 2, "BLUE");

    game_->addPlayer(player1);
    game_->addPlayer(player2);

    courseRunner = std::make_shared<GameRunner>(game_, gameScene_, initResourceMap_);
    initPlayerControls();

    setupPlayerStash();
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

    // luodaan pelaajille käsialueen luokka
    for (unsigned int i=0; i<game_->players().size(); ++i) {
        std::shared_ptr<Interface::Player> pl = game_->players().at(i);

        // Luodaan pari korttia ja annetaan ne pelaajalle
        for (int j=0; j<4; ++j) {
            std::shared_ptr<Interface::ActionCard> card = std::make_shared<Interface::ActionCard>();
            pl->addCard(card);
        }
    }
    gameScene_->createHandCards(game_->players().at(0)->cards());

    playerInTurn = player1;
    displayPlayerStats();
}

GameWindow::~GameWindow()
{
    delete gameui_;
}

void GameWindow::drawLocations()
{

    std::vector<std::shared_ptr<Interface::Location>> locvec = getLocations();
    std::shared_ptr<Interface::Location> currentLocation = nullptr;
    gameScene_->drawLocations(locvec);
}

void GameWindow::drawPlayerAgents(std::shared_ptr<Interface::Player> &player)
 {
    std::vector<agentItem*> agents = playerAgentItems_.at(player);
    gameScene_->drawAgents(agents);
 }

void GameWindow::drawItem(mapItem *item)
{
    gameScene_->drawItem(item);
}

const std::vector<std::shared_ptr<Interface::Location> > GameWindow::getLocations()
{
    return game_->locations();
}

void GameWindow::enablePlayerHand(std::shared_ptr<Interface::Player> player)
{
    std::vector<std::shared_ptr<Interface::Player>> players = game_->players();
    // Player must exist in game class
    if (player and std::find(players.begin(), players.end(), player) != players.end())
    {
        std::vector<std::shared_ptr<Interface::CardInterface>> cards = player->cards();
        std::vector<CardItem> carditems;
        for (unsigned int i = 0; i < cards.size(); ++i)
        {
            CardItem *carditem = new CardItem(cards.at(i), this);
            // adds card to the scene
            gameScene_->addItem(carditem);
        }
    }
}

void GameWindow::spawnAgent(std::shared_ptr<Interface::Player> &player)
{
    // Create agent interface, which holds all of the data of the card.
    // Ideally we want this to be handled by carddata class, which would use xml/JSON later on
    // For now we will just use some default generated stuff
    QString agname{"Perry"};
    std::shared_ptr<Interface::Agent> agentptr = std::make_shared<Interface::Agent>(agname + player->name(), player);
    agentptr->initAgentResources(initResourceMap_);

    agentItem* agenttiesine = new agentItem(agentptr);
    gameScene_->addItem(agenttiesine);

    playerAgentItems_.at(player).push_back(agenttiesine);
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

    gameScene_->turnInfo(current_round, playerInTurn);

}

void GameWindow::listAgents(std::shared_ptr<Interface::Player> player)
{
    gameui_->agentListWidget->clear();
    auto listOfAgents = playerAgentItems_.at(player);
    // TO DO: keksi miten agentItemien lista muutetaan shared_ptr<Agent>eiksi tai tee taas uusi map
    /*
    for (auto agent : listOfAgents) {
       gameui_->agentListWidget->addItem(agent->name());
    }
    */
}

void GameWindow::setupPlayerStash()
{
    for (auto player : game_->players()) {

        // Map for agents each player has to use (empty on initialization)
        playerAgentItems_.insert({player, {}});

        // Map for players and their in-game currenty (2 coins on initialization)
        playerWallets_.insert({player, 2});

        // Map for players and their collected councilor cards (empty on initialization)
        councilorCards_.insert({player, {}});
    }
}

void GameWindow::displayPlayerStats()
{
    current_round++;
    gameui_->currentRoundLabel->setText("Current round: " + QString::number(current_round));
    gameui_->playerNameLabel->setText(playerInTurn->name());
    gameui_->playerCoinsLabel->setText(QString::number(playerWallets_.at(playerInTurn)));

    listAgents(playerInTurn);
}

void GameWindow::initAreaResources()
{
    for (auto loc : game_->locations()) {
        QString resName = loc->name() + " item";
        Interface::CommonResource res(resName, loc, 0);
        std::pair<std::shared_ptr<Interface::Location>, Interface::CommonResource> pair(loc, res);
        initResourceMap_.insert(pair);
    }
}

void GameWindow::initPlayerControls()
{
    std::shared_ptr<Interface::ManualControl> mancontrol;
    courseRunner->setPlayerControl(player1, mancontrol);
    courseRunner->setPlayerControl(player2, mancontrol);
    courseRunner->run();
    courseRunner->testDebug();
}

void GameWindow::on_passButton_clicked()
{
    changeTurn();
}
