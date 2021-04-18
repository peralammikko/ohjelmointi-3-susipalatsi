#include "gamewindow.hh"
#include "ui_gamewindow.h"
#include "mapitem.hh"
#include "locationitem.hh"
#include "agent.hh"
#include "agentitem.hh"
#include "gamescene.hh"
#include <cmath>

// Kokeilu gamesetup
#include "gamesetup.hh"

#include "actioncard.hh"
#include "playerhand.hh"
#include "../Course/manualcontrol.h"


GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    gameui_(new Ui::GameWindow)
{
    gameui_->setupUi(this);

    // Declare the game first before gameScene, so we can give game_ to gameScene's constructor
    game_ = std::make_shared<Interface::Game>();
    game_->setActive(true);

    courseRunner = std::make_shared<GameRunner>(game_);

    gameScene_ = new GameScene(gameui_->graphicsView, game_);
    gameui_->graphicsView->setScene(gameScene_);
    gameui_->graphicsView->setMouseTracking(true);

    // Tell the game to start listening to the timer
    // TODO: move this after settings are selected or something
    gameTime_ = new QTimer(this);
    connect(gameTime_, SIGNAL(timeout()), gameScene_, SLOT(advance()));
    gameTime_->start(50);

    // Asetetaan graphicViewin ja ikkunan koot staattiseks ensalkuun
    gameui_->graphicsView->setFixedSize(1400, 800);
    // gameScene_->setSceneRect(-600,600,-350,350);
    this->setFixedSize(1450, 950);
    this->setWindowTitle("SUSIPALATSI: TEH GAME");

    /* GAME SETTINGS REPLACE
    // Luodaan location-oliot
    for (int i = 0; i < 6; i++) {
        std::shared_ptr<Interface::Location> location = std::make_shared<Interface::Location>(i, paikat_.at(i));
        game_->addLocation(location);
    }*

    std::vector<std::shared_ptr<Interface::Location>> locvec = game_->locations();
    gameScene_->drawLocations(locvec);

    player1 = std::make_shared<Interface::Player>(game_, 1, "RED");
    player2 = std::make_shared<Interface::Player>(game_, 2, "BLUE");

    game_->addPlayer(player1);
    game_->addPlayer(player2);

  //  initPlayerControls();

    setupPlayerStash();
    for (int i = 0 ; i < 3; i++) {
        spawnAgent(player1);
    }  
    for (int i = 0; i < 5; i++) {
        spawnAgent(player2);
    }*/
    // TODO: move logic and gamerunner init into gamesetup somehow
    logic_ = std::make_shared<Logic>(courseRunner, game_, gameScene_);
    GameSetup* setup = new GameSetup(gameScene_, game_, courseRunner,  logic_);

    // TODO: move this to logic and use game_'s turn based system instead
    /*
    if (current_round == 0) {
        drawPlayerAgents(player1);
    } else {
        drawPlayerAgents(player2);
    }*/


    // This is a hardcorded card generation and it does NOT draw from decks or anything.
    // It can be here until we get reward system in order
    for (unsigned int i=0; i<game_->players().size(); ++i) {
        std::shared_ptr<Interface::Player> pl = game_->players().at(i);
        for (int j=0; j<4; ++j) {
            std::shared_ptr<Interface::ActionCard> card = std::make_shared<Interface::ActionCard>();
            pl->addCard(card);


            //connect(carditem, &mapItem::mapItemMouseDragged, this, &GameScene::onMapItemMouseDragged);
            //connect(carditem, &mapItem::mapItemMouseReleased, this, &GameScene::onMapItemMouseDropped);
        }
    }


    displayPlayerStats();
    initAreaResources();
    gameScene_->resourceInfo(areaResourceMap);
}

GameWindow::~GameWindow()
{
    delete gameui_;
}

/*
void GameWindow::drawPlayerAgents(std::shared_ptr<Interface::Player> &player)
 {
    std::vector<agentItem*> agents = playerAgentItems_.at(player);
    gameScene_->drawAgents(agents);
 }*/

void GameWindow::drawItem(mapItem *item)
{
    gameScene_->drawItem(item);
}

const std::vector<std::shared_ptr<Interface::Location> > GameWindow::getLocations()
{
    return game_->locations();
}
/*
void GameWindow::spawnAgent(std::shared_ptr<Interface::Player> &player)
{
    // Create agent interface, which holds all of the data of the card.
    // Ideally we want this to be handled by carddata class, which would use xml/JSON later on
    // For now we will just use some default generated stuff
    QString agname{"Perry"};
    std::shared_ptr<Interface::Agent> agentptr = std::make_shared<Interface::Agent>(agname + player->name(), player);

    agentItem* agenttiesine = new agentItem(agentptr);
    connect(agenttiesine, &agentItem::actionDeclared, gameScene_, &GameScene::onActionDeclared);

    gameScene_->addItem(agenttiesine);
    //agenttiesine->setParent(gameScene_);

    playerAgentItems_.at(player).push_back(agenttiesine);
}*/

std::shared_ptr<Interface::Player> GameWindow::getPlayerInTurn()
{
    return game_->currentPlayer();
}

void GameWindow::changeTurn()
{
    // FIX THIS TO LOGIC
    game_->nextPlayer();
    displayPlayerStats();
    //current_round += 1;
    //gameScene_->turnInfo(current_round, game_->currentPlayer());
    gameScene_->turnInfo(0, game_->currentPlayer());

}

void GameWindow::listAgents(std::shared_ptr<Interface::Player> player)
{
    gameui_->agentListWidget->clear();
    auto listOfAgents = playerAgentItems_.at(player);
    for (auto agent : listOfAgents) {
       // Sori tää hajotetaan hetkeksi
       //gameui_->agentListWidget->addItem(agent->name());
    }
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
    // This is temporarily broken
    //current_round++;
    //gameui_->currentRoundLabel->setText("Current round: " + QString::number(game_));
    gameui_->playerNameLabel->setText(game_->currentPlayer()->name());
    //gameui_->playerCoinsLabel->setText(QString::number(playerWallets_.at(game_->currentPlayer())));
    //gameui_->councillorNumberLabel->setText(QString::number(councilorCards_.at(game_->currentPlayer()).size()) + " / 6");
    //listAgents(game_->currentPlayer());
}

void GameWindow::initAreaResources()
{
    int i = 1;
    for (auto loc : game_->locations()) {
        std::pair<std::shared_ptr<Interface::Location>, CommonResource> areaResourcePair;
        areaResourcePair = {loc, CommonResource(i)};
        areaResourceMap.insert(areaResourcePair);
        i++;
    }
}

void GameWindow::on_passButton_clicked()
{
    // TODO: set the player on auto-pass
    changeTurn();
}
